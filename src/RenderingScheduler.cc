
# include "RenderingScheduler.hh"

namespace fractsim {

  RenderingScheduler::RenderingScheduler():
    utils::CoreObject("rendering_scheduler"),

    m_statusLocker(),
    m_jobsAvailable(false),
    m_remaining(0u),

    m_jobsLocker(),
    m_jobs(),

    m_poolLocker(),
    m_poolRunning(false),
    m_threadsLocker(),
    m_threads()
  {
    setService("scheduler");
    // Create the threads associated to this object.
    createThreadPool();
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
    Guard guard2(m_poolLocker);
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

    // Wait for all threads to finish.
    Guard guard(m_threadsLocker);
    for (unsigned id = 0u ; id < m_threads.size() ; ++id) {
      m_threads[id].join();
    }

    m_threads.clear();
  }

  void
  RenderingScheduler::jobFetchingLoop(unsigned threadID) {
    // Assume we're running.
    bool running = true;

    log("Creating thread " + std::to_string(threadID) + " for scheduler pool", utils::Level::Notice);

    while (running) {
      Guard guard(m_poolLocker);
      running = m_poolRunning;

      if (!running) {
        break;
      }
    }

    log("Terminating thread " + std::to_string(threadID) + " for scheduler pool", utils::Level::Notice);
  }

}
