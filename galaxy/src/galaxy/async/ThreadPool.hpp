///
/// ThreadPool.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ASYNC_THREADPOOL_HPP_
#define GALAXY_ASYNC_THREADPOOL_HPP_

#include <semaphore>

#include "galaxy/async/Task.hpp"
#include "galaxy/meta/Globals.hpp"

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
			ThreadPool() noexcept;

			///
			/// Destructor.
			///
			~ThreadPool() noexcept;

			///
			/// Make task and queue.
			///
			/// \tparam Lambda Function Type. Not required.
			///
			/// \param func Lambda or function to call when task is executed. Is moved.
			///
			template<typename Lambda>
			void queue(Lambda&& func) noexcept;

			///
			/// Queue a task for the thread pool to execute.
			///
			/// \param task Pointer to task to queue.
			///
			void queue(std::shared_ptr<Task> task) noexcept;

			///
			/// \brief Stop all threads.
			///
			/// Lets threads finish first.
			///
			void stop() noexcept;

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
			bool m_destroyed;

			///
			/// Worker threads to do a task on.
			///
			std::vector<std::jthread> m_workers;

			///
			/// Task queue.
			///
			std::queue<std::shared_ptr<Task>> m_tasks;

			///
			/// Controls thread synchronization.
			///
			std::counting_semaphore<GALAXY_DEFAULT_THREAD_COUNT> m_sync;

			///
			/// Mutex to protect queue.
			///
			std::mutex m_mutex;

			///
			/// Control thread activity.
			///
			std::atomic<bool> m_running;
		};

		template<typename Lambda>
		inline void ThreadPool::queue(Lambda&& func) noexcept
		{
			std::shared_ptr task = std::make_shared<Task>();
			task->set(func);

			queue(task);
		}
	} // namespace async
} // namespace galaxy

#endif