///
/// Unix.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include "Unix.hpp"

#ifdef GALAXY_UNIX_PLATFORM

namespace galaxy
{
	namespace platform
	{
		void configure_terminal() noexcept
		{
			// Not needed on Unix.
		}
	} // namespace platform
} // namespace galaxy

#endif