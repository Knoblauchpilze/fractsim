
# include "RenderingScheduler.hh"

namespace fractsim {

  RenderingScheduler::RenderingScheduler():
    utils::CoreObject("rendering_scheduler"),

    m_poolLocker(),
    m_waiter(),
    m_poolWaiter(m_poolLocker, std::defer_lock),
    m_poolRunning(false),
    m_jobsAvailable(false),

    m_threadsLocker(),
    m_threads(),

    m_jobsLocker(),
    m_jobs(),
    m_batchIndex(0u)
  {
    setService("scheduler");
    // Create the threads associated to this object.
    createThreadPool();
  }

  void
  RenderingScheduler::notifyRenderingJobs() {
    // Protect from concurrent accesses.
    m_poolWaiter.lock();
    Guard guard(m_jobsLocker);

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
    m_poolWaiter.lock();
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
    // Protect from concurrent creation of the pool.
    Guard guard(m_threadsLocker);

    m_threads.resize(getThreadPoolSize());
    for (unsigned id = 0u ; id < m_threads.size() ; ++id) {
      m_threads[id] = std::thread(
        &RenderingScheduler::jobFetchingLoop,
        this,
        id
      );
    }

    // Start the pool.
    m_poolWaiter.lock();
    m_poolRunning = true;
  }

  void
  RenderingScheduler::terminateThreads() {
    log("Waiting for locker to terminate pool");
    m_poolWaiter.lock();

    // If no threads are created, nothing to do.
    if (!m_poolRunning) {
      log("Pool is not running, nothing to do");
      m_poolWaiter.unlock();
      return;
    }

    // Ask the threads to stop.
    log("Asked pool to stop running");
    m_poolRunning = false;
    m_poolWaiter.unlock();
    m_waiter.notify_all();

    // Wait for all threads to finish.
    Guard guard(m_threadsLocker);
    for (unsigned id = 0u ; id < m_threads.size() ; ++id) {
      log("Waiting for thread " + std::to_string(id) + " to terminate");
      m_threads[id].join();
    }

    log("Pool has shutdown");

    m_threads.clear();
  }

  void
  RenderingScheduler::jobFetchingLoop(unsigned threadID) {
    log("Creating thread " + std::to_string(threadID) + " for scheduler pool", utils::Level::Notice);

    // Create the locker to use to wait for job to do.
    Locker tLock(m_poolLocker);

    while (m_poolRunning) {
      // Loop to avoid spurious wakeups.
      while (m_poolRunning && !m_jobsAvailable) {
        m_waiter.wait(tLock);
      }

      // Check whether we need to process some jobs or exit the process.
      if (!m_poolRunning) {
        break;
      }

      // Attempt to retrieve a job to process.
      RenderingTileShPtr tile = nullptr;
      unsigned batch = 0u;
      {
        Guard guard(m_jobsLocker);

        if (!m_jobs.empty()) {
          tile = m_jobs.back();
          m_jobs.pop_back();
        }

        batch = m_batchIndex;
      }

      // If we could fetch something process it.
      if (tile != nullptr) {
        // TODO: Actually do some processing.
        log("Should process tile " + tile->getName() + " belonging to batch " + std::to_string(batch), utils::Level::Warning);

        // TODO: Handle the notification of the main thread that a job is finished.
      }
    }

    log("Terminating thread " + std::to_string(threadID) + " for scheduler pool", utils::Level::Notice);
  }

}
