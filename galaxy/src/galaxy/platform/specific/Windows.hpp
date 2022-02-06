///
/// Windows.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PLATFORM_SPECIFIC_WINDOWS_HPP_
#define GALAXY_PLATFORM_SPECIFIC_WINDOWS_HPP_

#include "galaxy/meta/Globals.hpp"

#ifdef GALAXY_WIN_PLATFORM

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