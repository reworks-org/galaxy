///
/// LuaPlatform.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/platform/Laptop.hpp"
#include "galaxy/platform/Platform.hpp"
#include "galaxy/platform/Subprocess.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	void Lua::inject_platform() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();

		lua.set("GALAXY_DEBUG_BUILD", GALAXY_DEBUG_BUILD);

#ifdef GALAXY_WIN_PLATFORM
		lua.set("GALAXY_WIN_PLATFORM", true);
		lua.set("GALAXY_UNIX_PLATFORM", false);
#elif GALAXY_UNIX_PLATFORM
		lua.set("GALAXY_WIN_PLATFORM", false);
		lua.set("GALAXY_UNIX_PLATFORM", true);
#endif

		auto sub_type      = lua.new_usertype<Subprocess>("Subprocess", sol::constructors<Subprocess(), Subprocess(std::string_view)>());
		sub_type["create"] = &Subprocess::create;
		sub_type["wait"]   = &Subprocess::wait;
		sub_type["kill"]   = &Subprocess::kill;

		lua.set_function("galaxy_seed_random", &platform::seed_random);
		lua.set_function("galaxy_set_metadata", &platform::set_metadata);
		lua.set_function("galaxy_set_hint", &platform::set_hint);

		// clang-format off
		lua.new_enum<SDL_PowerState>("SDL_PowerState",
		{
			{"SDL_POWERSTATE_ERROR", SDL_PowerState::SDL_POWERSTATE_ERROR},
			{"SDL_POWERSTATE_UNKNOWN", SDL_PowerState::SDL_POWERSTATE_UNKNOWN},
			{"SDL_POWERSTATE_ON_BATTERY", SDL_PowerState::SDL_POWERSTATE_ON_BATTERY},
			{"SDL_POWERSTATE_NO_BATTERY", SDL_PowerState::SDL_POWERSTATE_NO_BATTERY},
			{"SDL_POWERSTATE_CHARGING", SDL_PowerState::SDL_POWERSTATE_CHARGING},
			{"SDL_POWERSTATE_CHARGED", SDL_PowerState::SDL_POWERSTATE_CHARGED}
		});
		// clang-format on

		lua.set_function("galaxy_laptop_get_battery_remaining", &laptop::get_battery_remaining);
		lua.set_function("galaxy_laptop_get_battery_status", &laptop::get_battery_status);
	}
} // namespace galaxy
