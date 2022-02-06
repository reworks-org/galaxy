///
/// TimerAsync.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <future>

#include "galaxy/meta/Globals.hpp"

#include "TimerAsync.hpp"

namespace galaxy
{
	namespace async
	{
		TimerAsync::TimerAsync() noexcept
			: m_repeat {false}
			, m_delay {1000}
			, m_callback {nullptr}
		{
		}

		TimerAsync::~TimerAsync() noexcept
		{
			stop();
		}

		void TimerAsync::repeat(const bool repeat) noexcept
		{
			m_repeat = repeat;
		}

		void TimerAsync::start() noexcept
		{
			std::async(std::launch::async, [this]() {
				while (!m_repeat)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(m_delay));
					m_callback();
				}
			});
		}

		void TimerAsync::stop() noexcept
		{
			m_repeat = false;
		}
	} // namespace async
} // namespace galaxy