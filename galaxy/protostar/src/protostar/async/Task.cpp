///
/// Task.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "Task.hpp"

#include <chrono>
#include <thread>

///
/// Core namespace.
///
namespace pr
{
	Task::Task() noexcept
	{
		m_isFinished.set(false);
	}

	Task::~Task() noexcept
	{
		m_isFinished.set(true);
	}

	void Task::set(FunctionCallback&& function) noexcept
	{
		m_task = std::move(function);
	}

	void Task::exec(pr::ProtectedBool* isPoolDone) noexcept
	{
		m_task(isPoolDone);
		m_isFinished.set(true);
	}

	void Task::wait() noexcept
	{
		while (!m_isFinished.get())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}

	const bool Task::isFinished() noexcept
	{
		return m_isFinished.get();
	}
} // namespace pr