///
/// TimerAsync.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <future>

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

		TimerAsync::TimerAsync(const std::function<void(void)>& func, const std::uint32_t delay) noexcept
		{
			set(func, delay);
		}

		TimerAsync::~TimerAsync() noexcept
		{
			stop();
		}

		void TimerAsync::set(const std::function<void(void)>& func, const std::uint32_t delay) noexcept
		{
			m_callback = func;
			m_delay    = delay;
		}

		void TimerAsync::repeat(const bool repeat) noexcept
		{
			m_repeat = repeat;
		}

		void TimerAsync::start() noexcept
		{
			m_thread = std::jthread([&]() {
				do
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(m_delay));
					m_callback();
				} while (m_repeat);
			});
		}

		void TimerAsync::stop() noexcept
		{
			m_repeat = false;
			m_thread.request_stop();
			m_thread.join();
		}
	} // namespace async
} // namespace galaxy