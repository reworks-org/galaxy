///
/// Unix.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Unix.hpp"

#ifdef GALAXY_UNIX_PLATFORM

namespace galaxy
{
	namespace platform
	{
		void configure_terminal() const noexcept
		{
			// This function is stubbed on unix systems.
			return;
		}
	} // namespace platform
} // namespace galaxy

#endif
