///
/// Unix.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PLATFORM_SPECIFIC_UNIX_HPP_
#define GALAXY_PLATFORM_SPECIFIC_UNIX_HPP_

#include "galaxy/utils/Globals.hpp"

#ifdef GALAXY_UNIX_PLATFORM

namespace galaxy
{
	namespace platform
	{
		///
		/// Sets up debug terminal.
		///
		void configure_terminal() noexcept;
	} // namespace platform
} // namespace galaxy

#endif
#endif