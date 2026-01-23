///
/// LuaEntt.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt_sol/dispatcher/bond.hpp>
#include <entt_sol/registry/bond.hpp>
#include <entt_sol/scheduler/script_process.hpp>

#include "galaxy/platform/Pragma.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	void Lua::inject_entt() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();
		lua.require("dispatcher", sol::c_call<GALAXY_AUTOARG(&entt_sol::open_dispatcher)>, false);
		// TODO: lua["dispatcher"] = std::ref(dispatcher); // Make the dispatcher available to Lua

		lua.require("registry", sol::c_call<GALAXY_AUTOARG(&entt_sol::open_registry)>, false);
		// TODO: lua["registry"] = std::ref(registry); // Make the registry available to Lua

		lua.require("scheduler", sol::c_call<GALAXY_AUTOARG(&entt_sol::open_scheduler)>, false);
		// TODO: lua["scheduler"] = std::ref(scheduler); // Make the scheduler available to Lua
	}
} // namespace galaxy
