///
/// LuaSystems.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/systems/SystemManager.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	void Lua::inject_systems() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();

		auto sm_type            = lua.new_usertype<SystemManager>("SystemManager", sol::no_constructor);
		sm_type["add_system"]   = &SystemManager::add_system;
		sm_type["clear"]        = &SystemManager::clear;
		sm_type["resume"]       = &SystemManager::resume;
		sm_type["stack"]        = &SystemManager::stack;
		sm_type["suspend"]      = &SystemManager::suspend;
		sm_type["update"]       = &SystemManager::update;
		sm_type["is_suspended"] = &SystemManager::is_suspended;
	}
} // namespace galaxy
