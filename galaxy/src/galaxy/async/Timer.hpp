///
/// Timer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ASYNC_TIMER_HPP_
#define GALAXY_ASYNC_TIMER_HPP_

#include <functional>

namespace galaxy
{
	namespace async
	{
		///
		/// Synchronous timer class.
		///
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
			Timer(const std::function<void(void)>& func, const std::uint32_t delay);

			///
			/// Destructor.
			///
			~Timer();

			///
			/// Make function repeat itself instead of running once.
			///
			/// \param repeat True to repeat.
			///
			void repeat(const bool repeat);

			///
			/// Run a function on a precision timer.
			///
			/// \param func Function to call.
			/// \param delay Delay until function is called. In milliseconds.
			///
			void set(const std::function<void(void)>& func, const std::uint32_t delay);

			///
			/// Call to update timer count.
			///
			void update();

			///
			/// Start timer.
			///
			void start();

			///
			/// Stop timer.
			///
			void stop();

			///
			/// Is the timer finished?
			///
			/// \return True if timer is stopped (finished).
			///
			[[nodiscard]] bool stopped() const;

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
			/// Is function repeating?
			///
			bool m_repeat;

			///
			/// Is timer stopped.
			///
			bool m_stopped;

			///
			/// Time passed.
			///
			double m_time_passed;

			///
			/// Current delay on timer.
			///
			std::uint32_t m_delay;

			///
			/// Callback function.
			///
			std::function<void(void)> m_callback;
		};
	} // namespace async
} // namespace galaxy

#endif
