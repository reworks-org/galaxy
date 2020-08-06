///
/// ThreadPool.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_THREADPOOL_HPP_
#define PROTOSTAR_THREADPOOL_HPP_

#include <queue>
#include <semaphore>
#include <vector>

#include <jthread/jthread.hpp>

#include "protostar/async/ProtectedArithmetic.hpp"
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
		ThreadPool& operator=(ThreadPool&&) = delete;

		///
		/// Deleted copy assignment operator.
		///
		ThreadPool& operator=(const ThreadPool&) = delete;

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
		void create(const positive_integer auto& count);

		///
		/// Queue a task for the thread pool to execute.
		///
		/// \param task Pointer to task to queue.
		///
		void queue(Task* task);

		///
		/// Start all threads.
		///
		void start() noexcept;

		///
		/// Finish all threads.
		///
		void end();

	private:
		///
		/// Maximum threads avaliable for pool.
		///
		std::size_t m_max_threads;

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
		std::queue<pr::Task*> m_tasks;

		///
		/// Controls thread synchronization.
		///
		std::binary_semaphore m_sync;

		///
		/// Mutex to protect queue.
		///
		std::mutex m_mutex;

		///
		/// Control thread activity.
		///
		pr::ProtectedBool m_running;
	};
} // namespace pr

#endif