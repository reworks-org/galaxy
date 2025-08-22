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
#include "galaxy/platform/specific/Windows.hpp"
#elif GALAXY_UNIX_PLATFORM
#include "galaxy/platform/specific/Unix.hpp"
#endif

namespace galaxy
{
	namespace platform
	{
		void seed_random() noexcept;
	}
} // namespace galaxy

#endif
