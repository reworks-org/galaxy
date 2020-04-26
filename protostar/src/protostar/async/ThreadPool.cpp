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
namespace protostar
{
	ThreadPool::ThreadPool() noexcept
	{
		m_maxThreadCount = std::thread::hardware_concurrency();
	}

	ThreadPool::~ThreadPool() noexcept
	{
		m_isActive = false;

		for (auto& thread : m_workers)
		{
			thread.join();
		}
	}

	protostar::ThreadPool& ThreadPool::handle() noexcept
	{
		static protostar::ThreadPool s_thread;
		return s_thread;
	}

	void ThreadPool::create(const size_t count) noexcept
	{
		if (count > m_maxThreadCount)
		{
			m_maxThreadCount = count;
		}

		for (std::size_t it = 0; it < m_maxThreadCount; it++)
		{
			m_workers.emplace_back([&]()
			{
				while (m_isActive == true)
				{
					Task* task = nullptr;

					{
						std::unique_lock<std::mutex> l_lock(m_mutex);
						m_cv.wait(l_lock, [&] { return !m_tasks.empty(); });
						task = m_tasks.front();
						m_tasks.pop();
					}
					
					task->exec();
				}
			});
		}
	}

	void ThreadPool::queue(Task* task) noexcept
	{
		{
			std::unique_lock<std::mutex>(m_mutex);
			m_tasks.emplace(task);
		}

		m_cv.notify_one();
	}

	void ThreadPool::setActive(const bool isActive) noexcept
	{
		m_isActive = isActive;
	}
}