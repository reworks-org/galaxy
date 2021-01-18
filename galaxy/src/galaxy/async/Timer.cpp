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
		    : m_repeat {false}, m_delay {1000}
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

		void Timer::stop()
		{
			m_repeat = false;
			m_thread.request_stop();
			m_thread.join();
		}
	} // namespace async
} // namespace galaxy