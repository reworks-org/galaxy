///
/// ThreadPool.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"

#include "ThreadPool.hpp"

namespace galaxy
{
	namespace async
	{
		ThreadPool::ThreadPool()
			: m_is_destroyed {true}
			, m_sync {0}
		{
			m_running = true;

			m_max_threads = std::thread::hardware_concurrency();
			if (m_max_threads > MAX_THREADS_ALLOWED_TOTAL)
			{
				m_max_threads = MAX_THREADS_ALLOWED_TOTAL;
			}

			for (std::size_t i = 0; i < m_max_threads; i++)
			{
				// This is just storing the thread.
				m_workers.emplace_back([&]() {
					Task* task = nullptr;

					while (m_running)
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

		ThreadPool::~ThreadPool()
		{
			if (!m_is_destroyed)
			{
				finish();
			}
		}

		void ThreadPool::queue(Task* task) noexcept
		{
			m_mutex.lock();
			m_tasks.emplace(task);
			m_mutex.unlock();

			m_sync.release();
		}

		void ThreadPool::finish()
		{
			m_running = false;

			// Make sure tasks is not in use, then empty queue.
			// In case any tasks are left over.
			m_mutex.lock();
			while (!m_tasks.empty())
			{
				m_tasks.pop();
			}
			m_mutex.unlock();

			m_sync.release(m_max_threads);

			// Destroy all threads.
			for (auto& worker : m_workers)
			{
				worker.request_stop();
				worker.join();
			}

			m_workers.clear();
			m_is_destroyed = true;
		}
	} // namespace async
} // namespace galaxy