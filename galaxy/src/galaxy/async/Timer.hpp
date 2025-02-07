///
/// Timer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ASYNC_TIMER_HPP_
#define GALAXY_ASYNC_TIMER_HPP_

#include <future>

namespace galaxy
{
	namespace async
	{

		///
		/// Asynchronous timer class.
		///
		class Timer final
		{
		public:
			///
			/// Timer callback type.
			///
			using Function = std::move_only_function<void(void)>;

			///
			/// Constructor.
			///
			Timer() noexcept;

			///
			/// Set constructor.
			///
			/// \param func Function to call.
			/// \param delay Delay until function is called. In milliseconds.
			///
			Timer(Timer::Function&& func, const std::uint32_t delay) noexcept;

			///
			/// Destructor.
			///
			~Timer() noexcept;

			///
			/// Run a function on a precision timer.
			///
			/// \param func Function to call.
			/// \param delay Delay until function is called. In milliseconds.
			///
			void set(Timer::Function&& func, const std::uint32_t delay) noexcept;

			///
			/// Make function repeat itself instead of running once.
			///
			/// \param repeat True to repeat.
			///
			void repeat(const bool repeat) noexcept;

			///
			/// Start timer.
			///
			void start();

			///
			/// Stop timer.
			///
			void stop() noexcept;

			///
			/// Pause the timer.
			///
			/// \param True to pause, False to unpause.
			///
			void pause(const bool pause) noexcept;

			///
			/// Is the timer finished?
			///
			/// \return True if timer is stopped (finished).
			///
			[[nodiscard]]
			bool stopped() const noexcept;

			///
			/// Is the timer paused?
			///
			/// \return True if timer is paused.
			///
			[[nodiscard]]
			bool paused() const noexcept;

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
			Timer::Function m_callback;
		};
	} // namespace async
} // namespace galaxy

#endif
