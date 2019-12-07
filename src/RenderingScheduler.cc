
# include "RenderingScheduler.hh"

namespace fractsim {

  RenderingScheduler::RenderingScheduler():
    utils::CoreObject("rendering_scheduler"),

    m_statusLocker(),
    m_jobsAvailable(false),

    m_jobsLocker(),
    m_jobs()
  {
    setService("scheduler");
  }

}
