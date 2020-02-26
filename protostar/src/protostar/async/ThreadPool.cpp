///
/// ThreadPool.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "ThreadPool.hpp"

///
/// Core namespace.
///
namespace protostar
{
	ThreadPool::ThreadPool() noexcept
	{
		m_maxThreadCount = (std::thread::hardware_concurrency() - 1);
	}

	ThreadPool::ThreadPool(const std::size_t count)
	{
		m_maxThreadCount = (std::thread::hardware_concurrency() - 1);

		create(count);
	}

	ThreadPool::~ThreadPool() noexcept
	{
	}

	void ThreadPool::create(const size_t count)
	{
	}
}