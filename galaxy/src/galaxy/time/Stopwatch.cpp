///
/// Stopwatch.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <SDL3/SDL_timer.h>

#include "Stopwatch.hpp"

namespace galaxy
{
	Stopwatch::Stopwatch()
		: m_stopped {true}
		, m_paused {false}
		, m_repeat {false}
		, m_start_ticks {0}
		, m_paused_ticks {0}
		, m_delay {1000}
		, m_callback {nullptr}
	{
	}

	Stopwatch::~Stopwatch()
	{
		stop();
	}

	void Stopwatch::set(Stopwatch::Function&& func, const std::uint64_t delay)
	{
		m_callback = std::move(func);
		m_delay    = delay;
	}

	void Stopwatch::repeat(const bool repeat)
	{
		m_repeat = repeat;
	}

	void Stopwatch::start()
	{
		m_stopped = false;
		m_paused  = false;

		m_start_ticks  = SDL_GetTicksNS();
		m_paused_ticks = 0;
	}

	void Stopwatch::stop()
	{
		m_stopped = true;
		m_paused  = false;

		m_start_ticks  = 0;
		m_paused_ticks = 0;
	}

	void Stopwatch::pause()
	{
		if (!m_stopped && !m_paused)
		{
			m_paused = true;

			m_paused_ticks = SDL_GetTicksNS() - m_start_ticks;
			m_start_ticks  = 0;
		}
	}

	void Stopwatch::unpause()
	{
		if (!m_stopped && m_paused)
		{
			m_paused = false;

			m_start_ticks  = SDL_GetTicksNS() - m_paused_ticks;
			m_paused_ticks = 0;
		}
	}

	void Stopwatch::update()
	{
		if (!m_stopped && !m_paused)
		{
			if (get_time_passed() >= m_delay)
			{
				m_callback();

				if (!m_repeat)
				{
					stop();
				}
			}
		}
	}

	bool Stopwatch::stopped() const
	{
		return m_stopped;
	}

	bool Stopwatch::paused() const
	{
		return m_paused && !m_stopped;
	}

	std::uint64_t Stopwatch::get_time_passed() const noexcept
	{
		std::uint64_t time = 0;

		if (!m_stopped)
		{
			if (m_paused)
			{
				time = m_paused_ticks;
			}
			else
			{
				time = SDL_GetTicksNS() - m_start_ticks;
			}
		}

		// We need to convert because time_passed is in MS while
		// timer is in NS.
		return time / 1000000;
	}
} // namespace galaxy
