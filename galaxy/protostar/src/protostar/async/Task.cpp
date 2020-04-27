///
/// Task.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include <thread>
#include <chrono>

#include "Task.hpp"

///
/// Core namespace.
///
namespace protostar
{
	Task::Task() noexcept
	{
		m_isFinished.set(false);
	}

	Task::~Task() noexcept
	{
		m_isFinished.set(true);
	}

	void Task::exec(protostar::ProtectedBool* threadPoolFinished)
	{
		m_task(threadPoolFinished);
		m_isFinished.set(true);
	}

	void Task::waitUntilFinished() noexcept
	{
		while (!m_isFinished.get())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}
}