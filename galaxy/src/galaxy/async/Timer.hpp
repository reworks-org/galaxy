///
/// Timer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ASYNC_TIMER_HPP_
#define GALAXY_ASYNC_TIMER_HPP_

#include <chrono>
#include <functional>
#include <mutex>
#include <thread>

namespace galaxy
{
	namespace async
	{
		///
		/// Simple timer class.
		///
		class Timer final
		{
		public:
			///
			/// Constructor.
			///
			Timer() noexcept;

			///
			/// Destructor.
			///
			~Timer();

			///
			/// Make function repeat itself instead of running once.
			///
			/// \param repeat True to repeat.
			///
			void set_repeating(const bool repeat) noexcept;

			///
			/// \brief Run a function on a precision timer.
			///
			/// The function runs on a thread so do not run any thread dependent code.
			///
			/// \param func Function to call on thread.
			/// \param delay Delay until function is called. In milliseconds.
			///
			template<typename Lambda>
			void launch(Lambda&& func, int delay) noexcept;

			///
			/// \brief Stop timer.
			///
			/// Will not preserve data, launch must be called again.
			///
			void stop();

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
			/// Flag to check if timer stopped already.
			///
			bool m_stopped;

			///
			/// Is function repeating on thread.
			///
			bool m_repeat;

			///
			/// Current delay on timer.
			///
			int m_delay;

			///
			/// Thread to run task on.
			///
			std::jthread m_thread;

			///
			/// Callback function.
			///
			std::function<void(void)> m_callback;
		};

		template<typename Lambda>
		inline void Timer::launch(Lambda&& func, int delay) noexcept
		{
			m_callback = func;
			m_delay    = delay;
			m_thread   = std::jthread([&]() {
                                do
                                {
                                        std::this_thread::sleep_for(std::chrono::milliseconds(m_delay));
                                        m_callback();
                                } while (m_repeat);
                        });

			m_stopped = false;
		}
	} // namespace async
} // namespace galaxy

#endif