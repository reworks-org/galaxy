///
/// Timer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Timer.hpp"

namespace galaxy
{
	namespace async
	{
		Timer::Timer() noexcept
		    : m_repeat {false}, m_stopped {true}, m_time_passed {0.0}, m_delay {1000}, m_callback {nullptr}
		{
		}

		Timer::~Timer()
		{
			stop();
		}

		void Timer::set_repeating(const bool repeat) noexcept
		{
			m_repeat = repeat;
		}

		void Timer::update(const double dt)
		{
			if (!m_stopped)
			{
				m_time_passed += (dt * 1000.0);
				if (m_time_passed >= m_delay)
				{
					m_callback();
					if (m_repeat)
					{
						m_time_passed = 0.0;
					}
					else
					{
						stop();
					}
				}
			}
		}

		void Timer::start()
		{
			m_stopped = false;
		}

		void Timer::stop()
		{
			m_stopped     = true;
			m_time_passed = 0.0;
		}
	} // namespace async
} // namespace galaxy