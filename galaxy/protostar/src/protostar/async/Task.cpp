///
/// Task.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "Task.hpp"

#include <atomic_wait>
#include <chrono>
#include <thread>

///
/// Core namespace.
///
namespace pr
{
	Task::Task() noexcept
	    : m_done(false)
	{
	}

	Task::~Task() noexcept
	{
		m_done = true;
	}

	void Task::set(FunctionCallback&& func) noexcept
	{
		m_task = std::move(func);
	}

	void Task::exec() noexcept
	{
		m_task();
		m_done = true;
	}

	void Task::waitUntilDone()
	{
		std::atomic_wait(&m_done, false);
	}

	const bool Task::done() noexcept
	{
		return m_done;
	}
} // namespace pr