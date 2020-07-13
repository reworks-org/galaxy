///
/// ThreadPool.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_THREADPOOL_HPP_
#define PROTOSTAR_THREADPOOL_HPP_

#include <queue>
#include <future>
#include <vector>
#include <condition_variable>

#include "protostar/async/Task.hpp"

///
/// Core namespace.
///
namespace pr
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
		/// \brief Creates a number of threads.
		///
		/// You must call create() before using any functions.
		///
		/// \param count Amount of threads to create for pool. If invalid, sets to the default of 4.
		///				I.e. too big or less than 0.
		///
		void create(const size_t count = 4) noexcept;

		///
		/// Queue a task for the thread pool to execute.
		///
		/// \param task R-value task to queue.
		///
		void queue(Task* task) noexcept;

		///
		/// Set if threads are active or not.
		///
		/// \param isActive True if threads are active to work on tasks.
		///
		void setActive(const bool isActive) noexcept;

		///
		/// Cleanup threads used by threadpool.
		///
		void destroy() noexcept;

	private:
		///
		/// Deleted move constructor.
		///
		ThreadPool(ThreadPool&&) = delete;

		///
		/// Deleted copy constructor.
		///
		ThreadPool(const ThreadPool&) = delete;

		///
		/// Deleted move assignment operator.
		///
		ThreadPool& operator= (ThreadPool&&) = delete;

		///
		/// Deleted copy assignment operator.
		///
		ThreadPool& operator= (const ThreadPool&) = delete;

	private:
		///
		/// Maximum threads avaliable for pool.
		///
		std::size_t m_maxThreadCount;

		///
		/// Keeps track if threadpool has been destroyed.
		///
		bool m_isDestroyed;

		///
		/// Worker threads to do a task on.
		///
		std::vector<std::future<void>> m_workers;

		///
		/// Task queue.
		///
		std::queue<pr::Task*> m_tasks;

		///
		/// Condition variable for synchronization.
		///
		std::condition_variable m_cv;

		///
		/// Mutex to protect queue.
		///
		std::mutex m_mutex;

		///
		/// Atomic boolean to control thread activity.
		///
		pr::ProtectedBool m_isActive;
	};
}

#endif