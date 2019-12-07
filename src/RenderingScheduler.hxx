#ifndef    RENDERING_SCHEDULER_HXX
# define   RENDERING_SCHEDULER_HXX

# include "RenderingScheduler.hh"

namespace fractsim {

  inline
  void
  RenderingScheduler::notifyRenderingJobs() {
    // Protect from concurrent accesses.
    Guard guard(m_statusLocker);

    m_jobsAvailable = true;

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
    // TODO: Handle cancellation.
  }

}

#endif    /* RENDERING_SCHEDULER_HXX */
