
# include "RenderingScheduler.hh"

namespace fractsim {

  RenderingScheduler::RenderingScheduler():
    utils::CoreObject("rendering_scheduler"),

    m_poolLocker(),
    m_waiter(),
    m_poolRunning(false),
    m_jobsAvailable(false),

    m_threadsLocker(),
    m_threads(),

    m_jobsLocker(),
    m_jobs(),
    m_batchIndex(0u),

    m_resultsLocker(),
    m_resultsHandling(false),
    m_results(),
    m_resWaiter(),
    m_resultsThreadLocker(),
    m_resultsHandlingThread()
  {
    setService("scheduler");
    // Create the threads associated to this object.
    createThreadPool();
  }

  void
  RenderingScheduler::notifyRenderingJobs() {
    // Protect from concurrent accesses.
    UniqueGuard guard(m_poolLocker);
    Guard guard2(m_jobsLocker);

    // Determine whether some jobs have to be processed.
    if (m_jobs.empty()) {
      log(
        std::string("Tried to start jobs processing but none are defined"),
        utils::Level::Warning
      );

      return;
    }

    // Indicate that some jobs are available.
    m_jobsAvailable = true;

    // Notify working threads.
    m_waiter.notify_all();
  }

  void
  RenderingScheduler::enqueueJobs(std::vector<RenderingTileShPtr>& jobs) {
    // Protect from concurrent accesses.
    Guard guard(m_jobsLocker);

    if (!m_jobs.empty()) {
      error(
        std::string("Could not enqueue ") + std::to_string(jobs.size()) + "(s)",
        std::string("Still ") + std::to_string(m_jobs.size()) + " job(s) to process"
      );
    }

    m_jobs.clear();
    m_jobs.swap(jobs);
  }

  void
  RenderingScheduler::cancelJobs() {
    // Protect from concurrent accesses.
    log("Waiting for pool locker");
    UniqueGuard guard(m_poolLocker);
    log("Waiting for jobs locker");
    Guard guard2(m_jobsLocker);

    // Clear the internal queue so that no more jobs can be fetched.
    m_jobsAvailable = false;
    m_jobs.clear();

    log("Clearing " + std::to_string(m_jobs.size()) + " remaining job(s), next batch will be " + std::to_string(m_batchIndex));

    // Increment the batch index to mark any currently processing job
    // as invalid when it will complete.
    ++m_batchIndex;
  }

  void
  RenderingScheduler::createThreadPool() {
    // Create the results handling thread.
    {
      Guard guard(m_resultsLocker);
      m_resultsHandling = true;
    }
    {
      Guard guard(m_resultsThreadLocker);
      m_resultsHandlingThread = std::thread(
        &RenderingScheduler::resultsHandlingLoop,
        this
      );
    }

    // Protect from concurrent creation of the pool.
    Guard guard2(m_threadsLocker);

    m_threads.resize(getThreadPoolSize());
    for (unsigned id = 0u ; id < m_threads.size() ; ++id) {
      m_threads[id] = std::thread(
        &RenderingScheduler::jobFetchingLoop,
        this,
        id
      );
    }

    // Start the pool.
    UniqueGuard guard3(m_poolLocker);
    m_poolRunning = true;
  }

  void
  RenderingScheduler::terminateThreads() {
    m_poolLocker.lock();

    // If no threads are created, nothing to do.
    if (!m_poolRunning) {
      m_poolLocker.unlock();
      return;
    }

    // Ask the threads to stop.
    m_poolRunning = false;
    m_poolLocker.unlock();
    m_waiter.notify_all();

    // Wait for all threads to finish.
    Guard guard(m_threadsLocker);
    for (unsigned id = 0u ; id < m_threads.size() ; ++id) {
      m_threads[id].join();
    }

    m_threads.clear();

    // Now terminate the results handling thread.
    {
      m_resultsLocker.lock();

      // If the results thread is not started we don't have
      // to do anything.
      if (!m_resultsHandling) {
        m_resultsLocker.unlock();
        return;
      }

      // Stop the thread and wait for its termination.
      m_resultsHandling = false;
      m_resWaiter.notify_all();
      m_resultsLocker.unlock();

      Guard guard3(m_resultsThreadLocker);
      m_resultsHandlingThread.join();
    }
  }

  void
  RenderingScheduler::jobFetchingLoop(unsigned threadID) {
    log("Creating thread " + std::to_string(threadID) + " for scheduler pool", utils::Level::Notice);

    // Create the locker to use to wait for job to do.
    UniqueGuard tLock(m_poolLocker);

    while (m_poolRunning) {
      // Wait until either we are requested to stop or there are some
      // new jobs to process. Checking both conditions prevents us from
      // being falsely waked up (see spurious wakeups).
      m_waiter.wait(
        tLock,
        [&]() {
          return !m_poolRunning || m_jobsAvailable;
        }
      );

      // Check whether we need to process some jobs or exit the process.
      if (!m_poolRunning) {
        break;
      }

      // Attempt to retrieve a job to process.
      RenderingTileShPtr tile = nullptr;
      unsigned batch = 0u;
      unsigned remaining = 0u;

      {
        Guard guard(m_jobsLocker);

        if (!m_jobs.empty()) {
          tile = m_jobs.back();
          m_jobs.pop_back();
        }

        m_jobsAvailable = !m_jobs.empty();
        batch = m_batchIndex;

        remaining = m_jobs.size();
      }

      // Unlock the pool mutex so that we don't block other threads while
      // processing our chunk of job. This is what effectively allows for
      // concurrency.
      tLock.unlock();

      // If we could fetch something process it.
      if (tile != nullptr) {
        log(
          "Processing job for batch " + std::to_string(batch) + " in thread " + std::to_string(threadID) + " (remaining: " + std::to_string(remaining) + ")",
          utils::Level::Info
        );

        tile->render();

        // TODO: Handle the notification of the main thread that a job is finished.
        // Notify the main thread about the result.
        UniqueGuard guard(m_resultsLocker);
        m_results.push_back(tile);

        m_resWaiter.notify_one();
      }

      // Once the job is done, reacquire the mutex in order to re-wait on
      // the condition variable.
      tLock.lock();
    }

    log("Terminating thread " + std::to_string(threadID) + " for scheduler pool", utils::Level::Notice);
  }

  void
  RenderingScheduler::resultsHandlingLoop() {
    log("Creating results thread for scheduler pool", utils::Level::Notice);

    // Create the locker to use to wait for results to be processed.
    UniqueGuard rLock(m_resultsLocker);

    while (m_resultsHandling) {
      // Wait until either we are requested to stop or there are some
      // new results to analyze. Checking both conditions prevents us
      // from being falsely waked up (see spurious wakeups).
      m_resWaiter.wait(
        rLock,
        [&]() {
          return !m_resultsHandling || !m_results.empty();
        }
      );

      // Check whether we need to process some jobs or exit the process.
      if (!m_resultsHandling) {
        break;
      }

      log("Should process " + std::to_string(m_results.size()) + " result(s)", utils::Level::Warning);
      m_results.clear();

      // TODO: Post a repaint event with the area that just finished and
      // protect from concurrent accesses to `postEvent` as we are not in
      // the main events processing thread.
      // TODO: How to retrieve the information about the finished job ??
    }

    log("Terminating results thread for scheduler pool", utils::Level::Notice);
  }

}
