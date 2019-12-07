#ifndef    RENDERING_SCHEDULER_HXX
# define   RENDERING_SCHEDULER_HXX

# include "RenderingScheduler.hh"

namespace fractsim {

  inline
  RenderingScheduler::~RenderingScheduler() {
    terminateThreads();
  }

  inline
  unsigned
  RenderingScheduler::getThreadPoolSize() noexcept {
    return 3u;
  }

}

#endif    /* RENDERING_SCHEDULER_HXX */
