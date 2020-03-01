///
/// Task.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "Task.hpp"

///
/// Core namespace.
///
namespace protostar
{
	Task::Task() noexcept
	{
	}

	Task::~Task() noexcept
	{
	}

	void Task::exec()
	{
		m_task();
	}
}