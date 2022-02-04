///
/// TimerAsync.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ASYNC_TIMERASYNC_HPP_
#define GALAXY_ASYNC_TIMERASYNC_HPP_

#include <functional>

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
			TimerAsync() noexcept;

			///
			/// Set constructor.
			///
			/// \tparam Lambda Function type. Not required.
			///
			/// \param func Function to call on thread.
			/// \param delay Delay until function is called. In milliseconds.
			///
			template<typename Lambda>
			TimerAsync(Lambda&& func, const std::uint32_t delay) noexcept;

			///
			/// Destructor.
			///
			~TimerAsync() noexcept;

			///
			/// Make function repeat itself instead of running once.
			///
			/// \param repeat True to repeat.
			///
			void repeat(const bool repeat) noexcept;

			///
			/// \brief Run a function on a precision timer.
			///
			/// \tparam Lambda Function type. Not required.
			///
			/// The function runs on a thread so do not run any thread dependent code.
			///
			/// \param func Function to call on thread.
			/// \param delay Delay until function is called. In milliseconds.
			///
			template<typename Lambda>
			void set(Lambda&& func, const std::uint32_t delay) noexcept;

			///
			/// Start timer.
			///
			void start() noexcept;

			///
			/// \brief Stop a repeating timer.
			///
			/// Does nothing if timer is not set to repeat.
			///
			void stop() noexcept;

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
			/// Callback function.
			///
			std::function<void(void)> m_callback;
		};

		template<typename Lambda>
		inline TimerAsync::TimerAsync(Lambda&& func, const std::uint32_t delay) noexcept
		{
			set(func, delay);
		}

		template<typename Lambda>
		inline void TimerAsync::set(Lambda&& func, const std::uint32_t delay) noexcept
		{
			m_callback = func;
			m_delay    = delay;
		}
	} // namespace async
} // namespace galaxy

#endif