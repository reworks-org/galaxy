///
/// Unix.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_PLATFORM_SPECIFIC_UNIX_HPP_
#define GALAXY_PLATFORM_SPECIFIC_UNIX_HPP_

#include "galaxy/core/GalaxyConfig.hpp"

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