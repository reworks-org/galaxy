///
/// Timer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <BS_thread_pool.hpp>
#include <entt/locator/locator.hpp>

// #include "galaxy/core/Settings.hpp"

using namespace std::chrono_literals;

#include "Timer.hpp"

namespace galaxy
{
	namespace async
	{
		Timer::Timer() noexcept
			: m_stopped {true}
			, m_paused {false}
			, m_repeat {false}
			, m_delay {1000}
			, m_time_passed {0.0}
			, m_handle {}
			, m_callback {nullptr}
		{
		}

		Timer::Timer(Timer::Function&& func, const std::uint32_t delay) noexcept
			: m_stopped {true}
			, m_paused {false}
			, m_repeat {false}
			, m_delay {1000}
			, m_time_passed {0.0}
			, m_handle {}
			, m_callback {nullptr}
		{
			set(std::move(func), delay);
		}

		Timer::~Timer() noexcept
		{
			stop();
		}

		void Timer::set(Timer::Function&& func, const std::uint32_t delay) noexcept
		{
			m_callback = std::move(func);
			m_delay    = delay;
		}

		void Timer::repeat(const bool repeat) noexcept
		{
			m_repeat = repeat;
		}

		void Timer::start()
		{
			if (m_stopped && !m_paused)
			{
				m_stopped = false;
				m_paused  = false;

				auto& tp = entt::locator<BS::light_thread_pool>::value();
				m_handle = tp.submit_task([&]() {
					do
					{
						if (!m_paused)
						{
							std::this_thread::sleep_for(std::chrono::milliseconds(m_delay));
							m_callback();
						}
						else
						{
							// Prevent excess CPU usage.
							std::this_thread::sleep_for(0.1ms);
						}
					} while (m_repeat && !m_stopped);
				});
			}
		}

		void Timer::stop() noexcept
		{
			m_stopped     = true;
			m_repeat      = false;
			m_paused      = false;
			m_time_passed = 0.0;

			m_handle.get();
		}

		void Timer::pause(const bool pause) noexcept
		{
			m_paused = pause;
		}

		bool Timer::stopped() const noexcept
		{
			return m_stopped;
		}

		bool Timer::paused() const noexcept
		{
			return m_paused;
		}
	} // namespace async
} // namespace galaxy
