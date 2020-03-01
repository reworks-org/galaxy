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

	ThreadPool::ThreadPool(const std::size_t count)
	{
		create(count);
	}

	ThreadPool::~ThreadPool() noexcept
	{
		for (auto& thread : m_workers)
		{
			thread.join();
		}
	}

	void ThreadPool::create(const size_t count)
	{
		auto max = std::thread::hardware_concurrency();
		if (count > max)
		{
			m_maxThreadCount = max;
		}
		else
		{
			m_maxThreadCount = count;
		}

		for (std::size_t it = 0; it < m_maxThreadCount; it++)
		{
			m_workers.emplace_back([&]()
			{
				while (true)
				{
					Task task;

					{
						std::unique_lock<std::mutex> l_lock(m_mutex);
						m_cv.wait(l_lock, [&] { return !m_tasks.empty(); });

						task = m_tasks.front();
						m_tasks.pop();
					}

					task.exec();
				}
			});
		}
	}

	void ThreadPool::queue(Task&& task)
	{
		{
			std::unique_lock<std::mutex>(m_mutex);
			m_tasks.emplace(std::move(task));
		}

		m_cv.notify_one();
	}
}