///
/// Task.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include <atomic_wait>

#include "Task.hpp"

///
/// Core namespace.
///
namespace pr
{
	Task::Task() noexcept
	    : m_done {false}
	{
	}

	Task::~Task() noexcept
	{
		m_done = true;
	}

	void Task::set(std::function<void(void)>&& func) noexcept
	{
		m_task = std::move(func);
	}

	void Task::exec() noexcept
	{
		m_task();
		m_done = true;
	}

	void Task::wait_until_done()
	{
		std::atomic_wait(&m_done, false);
	}

	bool Task::is_done() noexcept
	{
		return m_done;
	}
} // namespace pr