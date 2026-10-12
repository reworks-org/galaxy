///
/// Platform.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PLATFORM_PLATFORM_HPP_
#define GALAXY_PLATFORM_PLATFORM_HPP_

#include <string>

#include "galaxy/platform/Pragma.hpp"

namespace galaxy
{
	namespace platform
	{
		///
		/// Seed the cstdlib rng algos.
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

		///
		/// Gets the current platform name as a string.
		///
		/// \return Returns the name of the platform. If the correct platform name is not available, returns a string beginning with the text "Unknown".
		///
		[[nodiscard]]
		std::string as_string() noexcept;
	} // namespace platform
} // namespace galaxy

#endif
