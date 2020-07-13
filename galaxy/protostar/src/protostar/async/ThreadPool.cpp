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
		:m_maxThreadCount(4), m_isDestroyed(false)
	{
		m_isActive.set(false);
	}

	ThreadPool::~ThreadPool() noexcept
	{
		if (!m_isDestroyed)
		{
			destroy();
		}
	}

	void ThreadPool::create(const size_t count) noexcept
	{
		if ((count == 0) || (count > std::thread::hardware_concurrency()))
		{
			m_maxThreadCount = 4;
		}
		else
		{
			m_maxThreadCount = count;
		}

		for (std::size_t it = 0; it < m_maxThreadCount; it++)
		{
			// This is just storing the thread.
			m_workers.emplace_back(std::move(std::async(std::launch::async, [&]()
			{
				// This part is on the thread.
				// This is a lambda.
				Task* task = nullptr;
				while (m_isActive.get())
				{
					task = nullptr;

					{
						// Wait until notification.
						std::unique_lock<std::mutex> l_lock(m_mutex);
						m_cv.wait(l_lock, [&]
						{
							return !m_tasks.empty() || (m_tasks.empty() && !m_isActive.get());
						});

						// Do task after notification.
						if (!m_tasks.empty())
						{
							task = m_tasks.front();
							m_tasks.pop();
						}
					}

					// Make sure a task was assigned.
					if (task != nullptr)
					{
						task->exec(&m_isActive);
					}
				}
			})));
		}
	}

	void ThreadPool::queue(Task* task) noexcept
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_tasks.emplace(task);
		}
		
		m_cv.notify_one();
	}

	void ThreadPool::setActive(const bool isActive) noexcept
	{
		m_isActive.set(isActive);
	}

	void ThreadPool::destroy() noexcept
	{
		// Turn off threads.
		m_isActive.set(false);

		// Make sure tasks is not in use, then empty queue.
		// In case any tasks are left over.
		{
			std::unique_lock<std::mutex> l_lock(m_mutex);
			while (!m_tasks.empty())
			{
				m_tasks.pop();
			}
		}

		// Notify all threads that they do not need to keep waiting.
		m_cv.notify_all();

		for (auto& future : m_workers)
		{
			// Resync threads.
			future.wait();
		}

		m_isDestroyed = true;
	}
}