///
/// ThreadPool.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ASYNC_THREADPOOL_HPP_
#define GALAXY_ASYNC_THREADPOOL_HPP_

#include <mutex>
#include <queue>
#include <semaphore>
#include <thread>
#include <vector>

#include "galaxy/async/Task.hpp"

#define MAX_THREADS_ALLOWED_TOTAL 16

namespace galaxy
{
	namespace async
	{
		///
		/// Creates a system defined count of threads to reuse for task based work.
		///
		class ThreadPool final
		{
		public:
			///
			/// Constructor.
			///
			ThreadPool();

			///
			/// Destructor.
			///
			~ThreadPool();

			///
			/// Queue a task for the thread pool to execute.
			///
			/// \param task Pointer to task to queue.
			///
			void queue(Task* task) noexcept;

			///
			/// Finish all threads.
			///
			void finish();

		private:
			///
			/// Copy constructor.
			///
			ThreadPool(const ThreadPool&) = delete;

			///
			/// Copy assignment operator.
			///
			ThreadPool& operator=(const ThreadPool&) = delete;

		private:
			///
			/// Keeps track if threadpool has been destroyed.
			///
			bool m_is_destroyed;

			///
			/// Worker threads to do a task on.
			///
			std::vector<std::jthread> m_workers;

			///
			/// Task queue.
			///
			std::queue<Task*> m_tasks;

			///
			/// Controls thread synchronization.
			///
			std::counting_semaphore<MAX_THREADS_ALLOWED_TOTAL> m_sync;

			///
			/// Mutex to protect queue.
			///
			std::mutex m_mutex;

			///
			/// Control thread activity.
			///
			std::atomic<bool> m_running;

			///
			/// Max threads allocated.
			///
			unsigned int m_max_threads;
		};
	} // namespace async
} // namespace galaxy

#endif