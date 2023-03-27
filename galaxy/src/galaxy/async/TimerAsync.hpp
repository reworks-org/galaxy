///
/// TimerAsync.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ASYNC_TIMERASYNC_HPP_
#define GALAXY_ASYNC_TIMERASYNC_HPP_

#include <functional>
#include <future>

namespace galaxy
{
	namespace async
	{
		///
		/// Asynchronous timer class.
		///
		class TimerAsync final
		{
		public:
			///
			/// Constructor.
			///
			TimerAsync();

			///
			/// Set constructor.
			///
			/// \param func Function to call on thread.
			/// \param delay Delay until function is called. In milliseconds.
			///
			TimerAsync(const std::function<void(void)>& func, const std::uint32_t delay);

			///
			/// Destructor.
			///
			~TimerAsync();

			///
			/// Make function repeat itself instead of running once.
			///
			/// \param repeat True to repeat.
			///
			void repeat(const bool repeat);

			///
			/// \brief Run a function on a precision timer.
			///
			/// The function runs on a thread so do not run any thread dependent code.
			///
			/// \param func Function to call on thread.
			/// \param delay Delay until function is called. In milliseconds.
			///
			void set(const std::function<void(void)>& func, const std::uint32_t delay);

			///
			/// Start timer.
			///
			void start();

			///
			/// \brief Stop a repeating timer.
			///
			/// Does nothing if timer is not set to repeat.
			///
			void stop();

		private:
			///
			/// Copy constructor.
			///
			TimerAsync(const TimerAsync&) = delete;

			///
			/// Copy assignment operator.
			///
			TimerAsync& operator=(const TimerAsync&) = delete;

		private:
			///
			/// Is function repeating?
			///
			std::atomic_bool m_repeat;

			///
			/// Current delay on timer.
			///
			std::uint32_t m_delay;

			///
			/// Thread running task.
			///
			std::future<void> m_handle;

			///
			/// Callback function.
			///
			std::function<void(void)> m_callback;
		};
	} // namespace async
} // namespace galaxy

#endif