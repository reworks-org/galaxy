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
		m_isFinished = false;
	}

	Task::~Task() noexcept
	{
	}

	void Task::exec()
	{
		m_task();
		m_isFinished = true;
	}

	void Task::waitUntilFinished() noexcept
	{
		while (!m_isFinished.load())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}
}