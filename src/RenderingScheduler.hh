#ifndef    RENDERING_SCHEDULER_HH
# define   RENDERING_SCHEDULER_HH

# include <mutex>
# include <vector>
# include <memory>
# include <core_utils/CoreObject.hh>
# include "RenderingTile.hh"

namespace fractsim {

  class RenderingScheduler: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new renderer state allowing to schedule the computation
       *          of a fractal.
       */
      RenderingScheduler();

      ~RenderingScheduler() = default;

      /**
       * @brief - Used to notify that some jobs have been inserted into the internal
       *          queue and that threads can thus start the computations.
       */
      void
      notifyRenderingJobs();

      /**
       * @brief - Used to enqueue the list of jobs provided in input into the internal
       *          queue. Note that no jobs should be available otherwise an error is
       *          raised (because it probably means that we don't finished the previous
       *          batch of jobs).
       *          The user should call the `notifyRenderingJobs` method to actually
       *          start the processing.
       * @param jobs - the list of jobs to enqueue. Note that the input vector is left
       *               in an undefined state after this function if it returns and thus
       *               should not be used afterwards.
       */
      void
      enqueueJobs(std::vector<RenderingTileShPtr>& jobs);

      /**
       * @brief - Used to cancel any existing jobs being processed for this scheduler.
       *          This function is needed in order to be able to call `enqueueJobs` again.
       */
      void
      cancelJobs();

    private:

      /**
       * @brief - Protect concurrent accesses to the status allowing to notify that
       *          some computing tasks are available.
       */
      std::mutex m_statusLocker;

      /**
       * @brief - Indicates whether there are some jobs to process. A `ŧrue` value
       *          tells that the internal queue for computing jobs has at least one
       *          value.
       */
      bool m_jobsAvailable;

      /**
       * @brief - Protect concurrent accesses to the jobs queue.
       */
      std::mutex m_jobsLocker;

      /**
       * @brief - The list of jobs currently available for processing.
       */
      std::vector<RenderingTileShPtr> m_jobs;
  };

  using RenderingSchedulerShPtr = std::shared_ptr<RenderingScheduler>;
}

# include "RenderingScheduler.hxx"

#endif    /* RENDERING_SCHEDULER_HH */
