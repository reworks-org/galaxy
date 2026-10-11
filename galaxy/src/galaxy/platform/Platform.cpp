///
/// Platform.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <cstdlib>
#include <ctime>

#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_init.h>

#include "Platform.hpp"

namespace galaxy
{
	namespace platform
	{
		void seed_random() noexcept
		{
			std::srand(static_cast<unsigned int>(std::time(nullptr)));
		}

		void set_metadata(const char* type, const char* value) noexcept
		{
			SDL_SetAppMetadataProperty(type, value);
		}

		void set_hint(const char* hint, const char* value) noexcept
		{
			SDL_SetHintWithPriority(hint, value, SDL_HINT_OVERRIDE);
		}
	} // namespace platform
} // namespace galaxy
