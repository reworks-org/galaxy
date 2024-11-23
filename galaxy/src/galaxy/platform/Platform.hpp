///
/// Platform.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PLATFORM_PLATFORM_HPP_
#define GALAXY_PLATFORM_PLATFORM_HPP_

#include "galaxy/core/Settings.hpp"

#ifdef GALAXY_WIN_PLATFORM
#include "galaxy/platform/specific/Windows.hpp"
#elif GALAXY_UNIX_PLATFORM
#include "galaxy/platform/specific/Unix.hpp"
#endif

namespace galaxy
{
	namespace platform
	{
		///
		/// Takes a screenshot of current screen.
		///
		/// \param file File to save to. Extension defines format.
		///
		void take_screenshot(const std::string& file) noexcept;

		///
		/// Open URL with default system browser (if available)
		///
		/// \param url Url to open in browser.
		///
		void open_url(const std::string& url) noexcept;
	} // namespace platform
} // namespace galaxy

#endif
