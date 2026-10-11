///
/// Laptop.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PLATFORM_LAPTOP_HPP_
#define GALAXY_PLATFORM_LAPTOP_HPP_

#include <cstdint>

#include "galaxy/platform/Pragma.hpp"

#ifdef GALAXY_WIN_PLATFORM
extern "C"
{
	// http://developer.amd.com/community/blog/2015/10/02/amd-enduro-system-for-developers/
	// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf

	inline __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
	inline __declspec(dllexport) std::uint32_t NvOptimusEnablement        = 0x00000001;
}
#elif
{
	inline int           AmdPowerXpressRequestHighPerformance = 1;
	inline std::uint32_t NvOptimusEnablement                  = 0x00000001;
}
#endif

namespace galaxy
{
	namespace laptop
	{
		///
		/// Get remaining battery charge of a laptop.
		///
		/// \param percent True to return as a percentage, otherwise return seconds left.
		///
		/// \return Percentage or seconds left.
		///
		[[nodiscard]]
		int get_battery_remaining(const bool percent = true) noexcept;

		///
		/// Get battery status.
		///
		/// \return SDL3 powerstate enum.
		///
		[[nodiscard]]
		SDL_PowerState get_battery_status() noexcept;
	} // namespace laptop
} // namespace galaxy

#endif
