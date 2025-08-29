///
/// Timer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TIME_TIMER_HPP_
#define GALAXY_TIME_TIMER_HPP_

#include <future>

namespace galaxy
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
		/// Destructor.
		///
		~Timer() noexcept;

		///
		/// Run a function on a precision timer.
		///
		/// \param func Function to call.
		/// \param delay Delay until function is called. In milliseconds.
		///
		void set(Timer::Function&& func, const std::uint64_t delay) noexcept;

		///
		/// Start timer.
		///
		/// \param repeat True to repeat. Defaults to false.
		///
		void start(const bool repeat = false);

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
		/// Has the timer been started.
		///
		std::atomic_bool m_started;

		///
		/// Is timer paused.
		///
		std::atomic_bool m_paused;

		///
		/// Is timer looping.
		///
		std::atomic_bool m_repeat;

		///
		/// Current delay on timer.
		///
		std::uint64_t m_delay;

		///
		/// Thread running task.
		///
		std::future<void> m_handle;

		///
		/// Callback function.
		///
		Timer::Function m_callback;
	};
} // namespace galaxy

#endif
