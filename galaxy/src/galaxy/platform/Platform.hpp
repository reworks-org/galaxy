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
		///
		///
		///
		void seed_random() noexcept;

		///
		/// \brief Sets metadata.
		///
		/// Do before init.
		///
		/// \param type SDL_PROP_APP_METADATA_*
		/// \param value Value for the SDL PROP.
		///
		void set_metadata(const char* type, const char* value) noexcept;

		///
		/// \brief Sets SDL hints.
		///
		/// Do before init.
		///
		/// \param type SDL_HINT_*
		/// \param value Value for the SDL HINT.
		///
		void set_hint(const char* hint, const char* value) noexcept;
	} // namespace platform
} // namespace galaxy

#endif
