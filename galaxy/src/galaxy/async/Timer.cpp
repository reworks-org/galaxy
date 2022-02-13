///
/// Timer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/utils/Globals.hpp"

#include "Timer.hpp"

namespace galaxy
{
	namespace async
	{
		Timer::Timer() noexcept
			: m_repeat {false}
			, m_stopped {true}
			, m_time_passed {0.0}
			, m_delay {1000}
			, m_callback {nullptr}
		{
		}

		Timer::Timer(const std::function<void(void)>& func, const std::uint32_t delay) noexcept
			: m_repeat {false}
			, m_stopped {true}
			, m_time_passed {0.0}
			, m_delay {1000}
			, m_callback {nullptr}
		{
			set(func, delay);
		}

		Timer::~Timer() noexcept
		{
			stop();
		}

		void Timer::set(const std::function<void(void)>& func, const std::uint32_t delay) noexcept
		{
			m_callback = func;
			m_delay    = delay;
		}

		void Timer::repeat(const bool repeat) noexcept
		{
			m_repeat = repeat;
		}

		void Timer::update() noexcept
		{
			if (!m_stopped)
			{
				m_time_passed += (GALAXY_DT * 1000.0);

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

		void Timer::start() noexcept
		{
			m_stopped = false;
		}

		void Timer::stop() noexcept
		{
			m_stopped     = true;
			m_time_passed = 0.0;
		}

		bool Timer::stopped() const noexcept
		{
			return m_stopped;
		}
	} // namespace async
} // namespace galaxy