///
/// ThreadPool.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_THREADPOOL_HPP_
#define PROTOSTAR_THREADPOOL_HPP_

#include <queue>
#include <thread>
#include <vector>

///
/// Core namespace.
///
namespace protostar
{
	///
	/// 
	///
	class ThreadPool
	{
	public:
		///
		/// Constructor.
		///
		ThreadPool() noexcept;

		///
		/// \brief Thread count constructor.
		///
		/// Can throw exceptions.
		///
		/// \param count Amount of threads to create for pool.
		///
		explicit ThreadPool(const std::size_t count);

		///
		/// Destructor.
		///
		~ThreadPool() noexcept;

		///
		/// \brief Creates a number of threads.
		///
		/// Can throw exceptions.
		///
		/// \param count Amount of threads to create for pool.
		///
		void create(const size_t count);

		///
		/// Queue a task for the thread pool to execute.
		///
		/// \param task r-value task to queue.
		///
		void queue(Task&& task);

		///
		/// \brief Executes all tasks.
		///
		/// Should be called in the update loop.
		/// Cannot throw exceptions.
		///
		void exec() noexcept;

	private:
		///
		/// Maximum threads avaliable for pool.
		///
		std::size_t m_maxThreadCount;

		///
		/// Worker threads to do a task on.
		///
		std::vector<std::thread> m_workers;

		///
		/// Task queue.
		///
		std::queue<protostar::Task> m_tasks;
	};
}

#endif