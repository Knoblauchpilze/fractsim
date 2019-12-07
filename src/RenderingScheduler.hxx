#ifndef    RENDERING_SCHEDULER_HXX
# define   RENDERING_SCHEDULER_HXX

# include "RenderingScheduler.hh"

namespace fractsim {

  inline
  RenderingScheduler::~RenderingScheduler() {
    terminateThreads();
  }

  inline
  void
  RenderingScheduler::notifyRenderingJobs() {
    // Protect from concurrent accesses.
    Guard guard(m_statusLocker);

    // Determine whether some jobs have to be processed.
    if (m_jobs.empty()) {
      log(
        std::string("Tried to start jobs processing but none are defined"),
        utils::Level::Warning
      );

      return;
    }

    m_jobsAvailable = true;
    m_remaining = m_jobs.size();

    // TODO: Actually start the threads.
  }

  inline
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

  inline
  void
  RenderingScheduler::cancelJobs() {
    // Protect from concurrent accesses.
    Guard guard(m_statusLocker);
    Guard guard2(m_jobsLocker);

    // Check if there's anything to cancel.
    if (m_jobs.empty() && m_remaining == 0u) {
      return;
    }

    log("Clearing " + std::to_string(m_jobs.size()) + " remaining job(s)");

    // Clear the internal queue so that no more jobs can be fetched.
    m_jobsAvailable = false;
    m_jobs.clear();

    // TODO: Handle some sort of status telling that termination of a job is
    // not relevant anymore.
  }

  inline
  unsigned
  RenderingScheduler::getThreadPoolSize() noexcept {
    return 3u;
  }

}

#endif    /* RENDERING_SCHEDULER_HXX */
