///
/// Platform.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PLATFORM_PLATFORM_HPP_
#define GALAXY_PLATFORM_PLATFORM_HPP_

#include "galaxy/platform/Pragma.hpp"

#ifdef GALAXY_WIN_PLATFORM

inline __declspec(dllexport) int          AmdPowerXpressRequestHighPerformance = 1;
inline __declspec(dllexport) unsigned int NvOptimusEnablement                  = 0x00000001;
#include "galaxy/platform/specific/Windows.hpp"

#elif GALAXY_UNIX_PLATFORM

inline int          AmdPowerXpressRequestHighPerformance = 1;
inline unsigned int NvOptimusEnablement                  = 0x00000001;
#include "galaxy/platform/specific/Unix.hpp"

#endif

namespace galaxy
{
	namespace platform
	{
		///
		/// Seed the cstdlib rng algos.
		///
		void seed_random() noexcept;
	} // namespace platform
} // namespace galaxy

#endif
