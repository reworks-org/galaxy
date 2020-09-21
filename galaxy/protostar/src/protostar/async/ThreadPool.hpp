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
#include <pulsar/Log.hpp>

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
	template<std::size_t max_threads>
	class ThreadPool final
	{
	public:
		///
		/// Constructor.
		///
		ThreadPool();

		///
		/// Copy constructor.
		///
		ThreadPool(const ThreadPool&) = delete;

		///
		/// Move constructor.
		///
		ThreadPool(ThreadPool&&) = delete;

		///
		/// Copy assignment operator.
		///
		ThreadPool& operator=(const ThreadPool&) = delete;

		///
		/// Move assignment operator.
		///
		ThreadPool& operator=(ThreadPool&&) = delete;

		///
		/// Destructor.
		///
		~ThreadPool();

		///
		/// Queue a task for the thread pool to execute.
		///
		/// \param task Pointer to task to queue.
		///
		void queue(Task* task);

		///
		/// Start all threads.
		///
		void start();

		///
		/// Finish all threads.
		///
		void end();

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
		std::queue<pr::Task*> m_tasks;

		///
		/// Controls thread synchronization.
		///
		std::counting_semaphore<max_threads> m_sync;

		///
		/// Mutex to protect queue.
		///
		std::mutex m_mutex;

		///
		/// Control thread activity.
		///
		pr::ProtectedBool m_running;
	};

	template<std::size_t max_threads>
	inline ThreadPool<max_threads>::ThreadPool()
	    : m_is_destroyed {true}, m_sync {0}
	{
		if (max_threads > std::thread::hardware_concurrency())
		{
			PL_LOG(PL_FATAL, "Threads exceed hardware max. Max is: {0}.", std::thread::hardware_concurrency());
		}

		m_running.set(false);

		for (std::size_t i = 0; i < max_threads; i++)
		{
			// This is just storing the thread.
			m_workers.emplace_back([&]() {
				Task* task = nullptr;

				while (m_running.get())
				{
					task = nullptr;

					// Wait until notification.
					m_sync.acquire();

					m_mutex.lock();
					if (!m_tasks.empty())
					{
						task = m_tasks.front();
						m_tasks.pop();
					}
					m_mutex.unlock();

					// Make sure a task was assigned.
					if (task != nullptr)
					{
						task->exec();
					}
				}
			});
		}
	}

	template<std::size_t max_threads>
	inline ThreadPool<max_threads>::~ThreadPool()
	{
		if (!m_is_destroyed)
		{
			end();
		}
	}

	template<std::size_t max_threads>
	inline void ThreadPool<max_threads>::queue(Task* task)
	{
		m_mutex.lock();
		m_tasks.emplace(task);
		m_mutex.unlock();

		m_sync.release();
	}

	template<std::size_t max_threads>
	inline void ThreadPool<max_threads>::start()
	{
		m_running.set(true);
	}

	template<std::size_t max_threads>
	inline void ThreadPool<max_threads>::end()
	{
		m_running.set(false);

		// Make sure tasks is not in use, then empty queue.
		// In case any tasks are left over.
		m_mutex.lock();
		while (!m_tasks.empty())
		{
			m_tasks.pop();
		}
		m_mutex.unlock();

		m_sync.release(max_threads);

		// Destroy all threads.
		for (auto&& worker : m_workers)
		{
			worker.request_stop();
			worker.join();
		}

		m_workers.clear();
		m_is_destroyed = true;
	}
} // namespace pr

#endif