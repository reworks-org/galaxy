///
/// Task.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Task.hpp"

namespace galaxy
{
	namespace async
	{
		Task::Task() noexcept
			: m_done {false}
		{
		}

		Task::Task(const std::function<void(void)>& func) noexcept
		{
			set(func);
		}

		Task::~Task() noexcept
		{
			m_done = true;

			std::atomic_notify_one(&m_done);
		}

		void Task::set(const std::function<void(void)>& func) noexcept
		{
			m_task = func;
		}

		void Task::exec() noexcept
		{
			m_task();

			m_done = true;
			std::atomic_notify_one(&m_done);
		}

		void Task::wait_until_done() noexcept
		{
			if (!m_done)
			{
				std::atomic_wait(&m_done, false);
			}
		}

		const bool Task::is_done() noexcept
		{
			return m_done;
		}
	} // namespace async
} // namespace galaxy