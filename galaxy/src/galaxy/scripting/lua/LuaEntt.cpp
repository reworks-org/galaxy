///
/// LuaEntt.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt_sol/dispatcher.hpp>
#include <entt_sol/registry.hpp>

#include "galaxy/core/ServiceLocator.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_entt()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			lua.require("registry", sol::c_call<decltype(&entt_sol::open_registry), &entt_sol::open_registry>, false);
			lua.require("dispatcher", sol::c_call<decltype(&entt_sol::open_dispatcher), &entt_sol::open_dispatcher>, false);

			auto entt_anytype     = lua.new_usertype<entt::any>("EnttAny", sol::no_constructor);
			entt_anytype["reset"] = &entt::any::reset;
			entt_anytype["type"]  = &entt::any::type;

			lua.new_usertype<entt::entity>("EnttEntity");
		}
	} // namespace lua
} // namespace galaxy
