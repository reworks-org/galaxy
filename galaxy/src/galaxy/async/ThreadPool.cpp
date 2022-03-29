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
		ThreadPool::ThreadPool() noexcept
			: m_destroyed {true}
			, m_sync {0}
		{
			m_running = true;

			for (std::size_t i = 0; i < GALAXY_THREADPOOL_COUNT; i++)
			{
				// This is just storing the thread.
				m_workers.emplace_back([&]() {
					std::shared_ptr<Task> task = nullptr;

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

		ThreadPool::~ThreadPool() noexcept
		{
			if (!m_destroyed)
			{
				stop();
			}
		}

		void ThreadPool::queue(const std::function<void(void)>& func) noexcept
		{
			std::shared_ptr task = std::make_shared<Task>();
			task->set(func);

			queue(task);
		}

		void ThreadPool::queue(std::shared_ptr<Task> task) noexcept
		{
			m_mutex.lock();
			m_tasks.emplace(task);
			m_mutex.unlock();

			m_sync.release();
		}

		void ThreadPool::stop() noexcept
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

			m_sync.release(GALAXY_THREADPOOL_COUNT);

			// Destroy all threads.
			for (auto& worker : m_workers)
			{
				worker.request_stop();
				worker.join();
			}

			m_workers.clear();
			m_destroyed = true;
		}
	} // namespace async
} // namespace galaxy