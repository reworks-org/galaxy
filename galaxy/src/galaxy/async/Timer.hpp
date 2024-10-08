///
/// Timer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ASYNC_TIMER_HPP_
#define GALAXY_ASYNC_TIMER_HPP_

#include <BS_thread_pool.hpp>
#include <entt/locator/locator.hpp>

#include "galaxy/core/Settings.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	namespace async
	{
		///
		/// Timer callback type.
		///
		using TimerCallback = std::move_only_function<void(void)>;

		///
		/// (A)synchronous timer class.
		///
		/// \tparam async If this timer should run in an async mode.
		///
		template<bool async = true>
		class Timer final
		{
		public:
			///
			/// Constructor.
			///
			Timer();

			///
			/// Set constructor.
			///
			/// \param func Function to call.
			/// \param delay Delay until function is called. In milliseconds.
			///
			Timer(TimerCallback&& func, const std::uint32_t delay);

			///
			/// Destructor.
			///
			~Timer();

			///
			/// Run a function on a precision timer.
			///
			/// \param func Function to call.
			/// \param delay Delay until function is called. In milliseconds.
			///
			void set(TimerCallback&& func, const std::uint32_t delay);

			///
			/// Make function repeat itself instead of running once.
			///
			/// \param repeat True to repeat.
			///
			void repeat(const bool repeat);

			///
			/// Start timer.
			///
			void start();

			///
			/// Stop timer.
			///
			void stop();

			///
			/// Pause the timer.
			///
			void pause();

			///
			/// Resume the timer.
			///
			void unpause();

			///
			/// \brief Call to update timer count.
			///
			/// You only need to call this if the timer is not running in async mode.
			///
			constexpr typename std::enable_if<async, void>::type update();

			///
			/// Is the timer finished?
			///
			/// \return True if timer is stopped (finished).
			///
			[[nodiscard]]
			bool stopped() const;

			///
			/// Is the timer paused?
			///
			/// \return True if timer is paused.
			///
			[[nodiscard]]
			bool paused() const;

		private:
			///
			/// Copy constructor.
			///
			Timer(const Timer&) = delete;

			///
			/// Copy assignment operator.
			///
			Timer& operator=(const Timer&) = delete;

		private:
			///
			/// Is timer stopped.
			///
			std::atomic_bool m_stopped;

			///
			/// Is timer paused.
			///
			std::atomic_bool m_paused;

			///
			/// Is function repeating?
			///
			std::atomic_bool m_repeat;

			///
			/// Current delay on timer.
			///
			std::uint32_t m_delay;

			///
			/// Time passed.
			///
			double m_time_passed;

			///
			/// Thread running task.
			///
			std::future<void> m_handle;

			///
			/// Callback function.
			///
			TimerCallback m_callback;
		};

		template<bool async>
		inline Timer<async>::Timer()
			: m_stopped {true}
			, m_paused {false}
			, m_repeat {false}
			, m_delay {1000}
			, m_time_passed {0.0}
			, m_handle {}
			, m_callback {nullptr}
		{
		}

		template<bool async>
		inline Timer<async>::Timer(TimerCallback&& func, const std::uint32_t delay)
			: m_stopped {true}
			, m_paused {false}
			, m_repeat {false}
			, m_delay {1000}
			, m_time_passed {0.0}
			, m_handle {}
			, m_callback {nullptr}
		{
			set(func, delay);
		}

		template<bool async>
		inline Timer<async>::~Timer()
		{
			stop();
		}

		template<bool async>
		inline void Timer<async>::set(TimerCallback&& func, const std::uint32_t delay)
		{
			m_callback = std::move(func);
			m_delay    = delay;
		}

		template<bool async>
		inline void Timer<async>::repeat(const bool repeat)
		{
			m_repeat = repeat;
		}

		template<bool async>
		inline void Timer<async>::start()
		{
			m_stopped = false;
			m_paused  = false;

			if constexpr (async)
			{
				auto& tp = entt::locator<BS::thread_pool>::value();
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

		template<bool async>
		inline void Timer<async>::stop()
		{
			m_stopped     = true;
			m_repeat      = false;
			m_paused      = false;
			m_time_passed = 0.0;

			if constexpr (async)
			{
				m_handle.get();
			}
		}

		template<bool async>
		inline void Timer<async>::pause()
		{
			m_paused = true;
		}

		template<bool async>
		inline void Timer<async>::unpause()
		{
			m_paused = false;
		}

		template<bool async>
		inline constexpr typename std::enable_if<async, void>::type Timer<async>::update()
		{
			if (!m_stopped && !m_paused)
			{
				m_time_passed += (settings::dt() * 1000.0);

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

		template<bool async>
		inline bool Timer<async>::stopped() const
		{
			return m_stopped;
		}

		template<bool async>
		inline bool Timer<async>::paused() const
		{
			return m_paused;
		}
	} // namespace async
} // namespace galaxy

#endif
