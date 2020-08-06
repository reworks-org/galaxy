///
/// ThreadPool.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "protostar/async/Task.hpp"

#include "ThreadPool.hpp"

///
/// Core namespace.
///
namespace pr
{
	ThreadPool::ThreadPool() noexcept
	    : m_max_threads {8}, m_is_destroyed {true}
	{
		m_running.set(false);
	}

	ThreadPool::~ThreadPool() noexcept
	{
		if (!m_is_destroyed)
		{
			end();
		}
	}

	void ThreadPool::create(const positive_integer auto& count)
	{
		if (!(count == 0 || count > std::thread::hardware_concurrency()))
		{
			m_max_threads = count;
		}

		for (std::size_t i = 0; i < m_max_threads; i++)
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

	void ThreadPool::queue(Task* task)
	{
		m_mutex.lock();
		m_tasks.emplace(task);
		m_mutex.unlock();

		m_sync.release();
	}

	void ThreadPool::start() noexcept
	{
		m_running.set(true);
	}

	void ThreadPool::end()
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

		// Notify all threads that they do not need to keep waiting.
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
} // namespace pr