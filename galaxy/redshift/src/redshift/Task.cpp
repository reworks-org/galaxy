///
/// Task.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include <future>

#include "Task.hpp"

namespace rs
{
	Task::Task()
		:m_isCompleted(false)
	{
	}	

	Task::~Task()
	{
		// Make sure its unlocked upon destruction.
		m_mutex.unlock();
	}

	void Task::setTask(const std::function<void(bool*)>& task)
	{
		m_task = task;
	}

	auto Task::run()
	{
		// Protect resources.
		std::lock_guard<std::mutex> l_lock(m_mutex);

		auto future = std::async(std::launch::async, m_task, &m_isCompleted);
		return std::move(future);
	}

	const bool Task::isCompleted() const noexcept
	{
		return m_isCompleted;
	}
}