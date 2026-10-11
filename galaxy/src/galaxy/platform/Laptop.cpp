///
/// Laptop.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <SDL3/SDL_power.h>

#include "Laptop.hpp"

namespace galaxy
{
	namespace laptop
	{
		int get_battery_remaining(const bool percent) noexcept
		{
			int seconds    = 0;
			int percentage = 0;

			SDL_GetPowerInfo(&seconds, &percentage);
			return percent ? percentage : seconds;
		}

		SDL_PowerState get_battery_status() noexcept
		{
			int seconds    = 0;
			int percentage = 0;

			return SDL_GetPowerInfo(&seconds, &percentage);
		}
	} // namespace laptop
} // namespace galaxy
