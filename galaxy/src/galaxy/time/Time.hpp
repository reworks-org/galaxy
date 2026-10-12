///
/// Time.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UTILS_TIME_HPP_
#define GALAXY_UTILS_TIME_HPP_

#include <chrono>

namespace galaxy
{
	namespace time
	{
		///
		/// Current local time.
		///
		/// \return Time as a chrono duration.
		///
		[[nodiscard]]
		auto now() noexcept -> std::chrono::local_time<std::chrono::system_clock::duration>;

		///
		/// Time since epoch.
		///
		/// \return Nanoseconds.
		///
		[[nodiscard]]
		std::uint64_t time_since_epoch() noexcept;

		///
		/// Set galaxy delta time.
		///
		/// \param dt Delta-Time.
		///
		void dt(const double dt) noexcept;

		///
		/// Get galaxy delta time.
		///
		/// \return Current lag from gameloop.
		///
		[[nodiscard]]
		double dt() noexcept;

		///
		/// Delay the current thread for x time.
		///
		/// \param milli Number of milliseconds to wait.
		///
		void delay(const int milli) noexcept;

		///
		/// Converts milliseconds to nanoseconds.
		///
		/// \param ms Milliseconds.
		///
		/// \return Nanoseconds.
		///
		[[nodiscard]]
		std::uint64_t ms_to_ns(const std::uint64_t ms) noexcept;

		///
		/// Converts nanoseconds to milliseconds.
		///
		/// \param ns Nanoseconds.
		///
		/// \return Milliseconds.
		///
		[[nodiscard]]
		std::uint64_t ns_to_ms(const std::uint64_t ns) noexcept;

		///
		/// Converts nanoseconds to seconds.
		///
		/// \param ns Nanoseconds.
		///
		/// \return Seconds.
		///
		[[nodiscard]]
		std::uint64_t ns_to_s(const std::uint64_t ns) noexcept;

		///
		/// Converts nanoseconds to microseconds.
		///
		/// \param ns Nanoseconds.
		///
		/// \return Microseconds.
		///
		[[nodiscard]]
		std::uint64_t ns_to_us(const std::uint64_t ns) noexcept;

		///
		/// Converts seconds to nanoseconds.
		///
		/// \param s Seconds.
		///
		/// \return Nanoseconds.
		///
		[[nodiscard]]
		std::uint64_t s_to_ns(const std::uint64_t s) noexcept;

		///
		/// Converts microseconds to nanoseconds.
		///
		/// \param us Microseconds.
		///
		/// \return Nanoseconds.
		///
		[[nodiscard]]
		std::uint64_t us_to_ns(const std::uint64_t us) noexcept;
	} // namespace time
} // namespace galaxy

#endif
