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
	} // namespace time
} // namespace galaxy

#endif
