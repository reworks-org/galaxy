///
/// Stopwatch.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TIME_STOPWATCH_HPP_
#define GALAXY_TIME_STOPWATCH_HPP_

#include <functional>

namespace galaxy
{
	///
	/// Synchronous stopwatch.
	///
	class Stopwatch final
	{
	public:
		///
		/// Stopwatch callback type.
		///
		using Function = std::move_only_function<void(void)>;

		///
		/// Constructor.
		///
		Stopwatch();

		///
		/// Destructor.
		///
		~Stopwatch();

		///
		/// Run a function on a precision Stopwatch.
		///
		/// \param func Function to call.
		/// \param delay Delay until function is called. In milliseconds.
		///
		void set(Stopwatch::Function&& func, const std::uint64_t delay);

		///
		/// Make function repeat itself instead of running once.
		///
		/// \param repeat True to repeat.
		///
		void repeat(const bool repeat);

		///
		/// Start Stopwatch.
		///
		void start();

		///
		/// Stop Stopwatch.
		///
		void stop();

		///
		/// Pause the Stopwatch.
		///
		void pause();

		///
		/// Resume the Stopwatch.
		///
		void unpause();

		///
		/// \brief Call this if you want to trigger the callback after delay has passed.
		///
		/// Not required if you just want to get time passed.
		/// Stops the stopwatch unless repeat is set.
		///
		void update();

		///
		/// Is the Stopwatch finished?
		///
		/// \return True if Stopwatch is stopped (finished).
		///
		[[nodiscard]]
		bool stopped() const;

		///
		/// Is the Stopwatch paused?
		///
		/// \return True if Stopwatch is paused.
		///
		[[nodiscard]]
		bool paused() const;

		///
		/// Time passed in milliseconds.
		///
		/// \return Milliseconds.
		///
		[[nodiscard]]
		std::uint64_t get_time_passed() const noexcept;

	private:
		///
		/// Copy constructor.
		///
		Stopwatch(const Stopwatch&) = delete;

		///
		/// Copy assignment operator.
		///
		Stopwatch& operator=(const Stopwatch&) = delete;

	private:
		///
		/// Is Stopwatch stopped.
		///
		bool m_stopped;

		///
		/// Is Stopwatch paused.
		///
		bool m_paused;

		///
		/// Is function repeating?
		///
		bool m_repeat;

		///
		/// The clock time when the timer started.
		///
		std::uint64_t m_start_ticks;

		///
		/// The ticks stored when the timer was paused.
		///
		std::uint64_t m_paused_ticks;

		///
		/// Duration before callback is invoked.
		///
		std::uint64_t m_delay;

		///
		/// Callback function.
		///
		Stopwatch::Function m_callback;
	};
} // namespace galaxy

#endif
