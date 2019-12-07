#ifndef    RENDERING_SCHEDULER_HH
# define   RENDERING_SCHEDULER_HH

# include <mutex>
# include <thread>
# include <vector>
# include <memory>
# include <condition_variable>
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

      /**
       * @brief - Used to destroy the object and terminate all the threads used to
       *          process the jobs.
       */
      ~RenderingScheduler();

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
       * @brief - Provide a default size of the size of the thread pool to create to perform
       *          the jobs processing. The larger this value the more threads will be created
       *          in order to be ready for processing jobs.
       * @return - a value indicating how many threads will be created by this scheduler.
       */
      unsigned
      getThreadPoolSize() noexcept;

      /**
       * @brief - Used to create the thread pool used by this scheduler to perform the user's
       *          computations. The threads are created while building this scheduler and are
       *          waiting for jobs to be enqueued.
       *          The number of threads to create is retrieved from the static method providing
       *          a default size for the pool.
       */
      void
      createThreadPool();

      /**
       * @brief - Used to terminate the threads associated to the thread pool. This is typically
       *          called upon destroying the scheduler.
       */
      void
      terminateThreads();

      /**
       * @brief - Used as a thread loop method when creating the pool. This method will be executed
       *          by each individual thread and handles the querying of the jobs, processing itself,
       *          notification of the results and termination when needed.
       *          This method takes an identifier that is used to identify the jobs processed by the
       *          thread.
       * @param threadID - a provided counter identifying this thread. Nothing fancy but it allows
       *                   to easily determine from which thread the completed jobs come from.
       */
      void
      jobFetchingLoop(unsigned threadID);

    private:

      /**
       * @brief- Convenience define to refer to the type of locker to protect the pool
       *         running statis from concurrent accesses.
       */
      using Mutex = std::mutex;

      /**
       * @brief - Convenience define to refer to a unique lock on the mutex used to
       *          protect the pool's running status.
       */
      using UniqueGuard = std::unique_lock<Mutex>;

      /**
       * @brief - A mutex protecting concurrent accesses to the thread composing the
       *          pool. Typically used to start or stop the thread pool.
       */
      Mutex m_poolLocker;

      /**
       * @brief - Condition variable used to put threads of the pool to sleep as long
       *          as no jobs are provided and the pool does not need to be terminated.
       */
      std::condition_variable m_waiter;

      /**
       * @brief - Keep track of whether the pool is running. As long as this value is
       *          `true` individual threads can continue fetching information and wait
       *          for jobs.
       */
      bool m_poolRunning;

      /**
       * @brief - Indicates whether there are some jobs to process. A `ŧrue` value
       *          tells that the internal queue for computing jobs has at least one
       *          value. We protect this boolean behind the same locker (i.e. the
       *          `m_poolLocker` one) as the `m_poolRunning` boolean because we want
       *          threads to be notified either when the pool needs to be terminated
       *          or when some new jobs are available.
       */
      bool m_jobsAvailable;

      /**
       * @brief - Protect concurrent accesses to the array of threads.
       */
      Mutex m_threadsLocker;

      /**
       * @brief - The threads used by the pool. When the pool is up and running there
       *          should be `getThreadPoolSize` threads registered in the vector. A
       *          termination of the pool destroys the thread but in general they should
       *          not be accessed directly.
       */
      std::vector<std::thread> m_threads;

      /**
       * @brief - Protect concurrent accesses to the jobs queue and related properties.
       */
      Mutex m_jobsLocker;

      /**
       * @brief - The list of jobs currently available for processing.
       */
      std::vector<RenderingTileShPtr> m_jobs;

      /**
       * @brief - An index identifying the current batch of jobs being fed to the
       *          threads. Any completion related to another batch will be discarded
       *          as it's probably irrelevant anymore.
       */
      unsigned m_batchIndex;
  };

  using RenderingSchedulerShPtr = std::shared_ptr<RenderingScheduler>;
}

# include "RenderingScheduler.hxx"

#endif    /* RENDERING_SCHEDULER_HH */
