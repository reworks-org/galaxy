///
/// TimerAsync.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <future>

#include <BS_thread_pool.hpp>

#include "galaxy/core/ServiceLocator.hpp"

#include "TimerAsync.hpp"

namespace galaxy
{
	namespace async
	{
		TimerAsync::TimerAsync()
			: m_repeat {false}
			, m_delay {1000}
			, m_callback {nullptr}
		{
		}

		TimerAsync::TimerAsync(const std::function<void(void)>& func, const std::uint32_t delay)
		{
			set(func, delay);
		}

		TimerAsync::~TimerAsync()
		{
			stop();
		}

		void TimerAsync::set(const std::function<void(void)>& func, const std::uint32_t delay)
		{
			m_callback = func;
			m_delay    = delay;
		}

		void TimerAsync::repeat(const bool repeat)
		{
			m_repeat = repeat;
		}

		void TimerAsync::start()
		{
			auto& tp = core::ServiceLocator<BS::thread_pool>::ref();
			m_handle = tp.submit([&]() {
				do
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(m_delay));
					m_callback();
				} while (m_repeat);
			});
		}

		void TimerAsync::stop()
		{
			m_repeat = false;
			m_handle.get();
		}
	} // namespace async
} // namespace galaxy