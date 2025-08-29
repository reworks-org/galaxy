///
/// Timer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <BS_thread_pool.hpp>
#include <entt/locator/locator.hpp>

#include "galaxy/utils/Async.hpp"

#include "Timer.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	Timer::Timer() noexcept
		: m_started {false}
		, m_paused {false}
		, m_repeat {false}
		, m_delay {1000}
		, m_handle {}
		, m_callback {nullptr}
	{
	}

	Timer::~Timer() noexcept
	{
		stop();
	}

	void Timer::set(Timer::Function&& func, const std::uint64_t delay) noexcept
	{
		m_callback = std::move(func);
		m_delay    = delay;
	}

	void Timer::start(const bool repeat)
	{
		m_repeat = repeat;
		if (!m_started)
		{
			m_started = true;
			m_paused  = false;

			auto& tp = entt::locator<BS::light_thread_pool>::value();
			m_handle = tp.submit_task([&]() {
				do
				{
					if (!m_paused && m_started)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(m_delay));
						m_callback();
					}
					else
					{
						// Prevent excess CPU usage.
						std::this_thread::sleep_for(0.01ms);
					}
				} while (m_repeat);
			});
		}
	}

	void Timer::stop() noexcept
	{
		m_started = false;
		m_paused  = false;
		m_repeat  = false;

		while (!async::is_work_done(m_handle))
		{
			std::this_thread::sleep_for(0.01ms);
		}
	}

	void Timer::pause(const bool pause) noexcept
	{
		m_paused = pause;
	}

	bool Timer::stopped() const noexcept
	{
		return !m_started;
	}

	bool Timer::paused() const noexcept
	{
		return m_paused && m_started;
	}
} // namespace galaxy
