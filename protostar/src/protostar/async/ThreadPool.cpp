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
			m_workers.emplace_back([](protostar::Task& task)
			{
				task.exec();
			});
		}
	}

	void ThreadPool::queue(Task&& task)
	{
		m_tasks.emplace(std::move(task));
	}

	void ThreadPool::exec() noexcept
	{
		m_tasks.
	}
}