///
/// LuaEntt.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#define AUTO_ARG(x) decltype(x), x

#include <entt_sol/dispatcher.hpp>
#include <entt_sol/registry.hpp>
#include <entt_sol/script_process.hpp>

#include "galaxy/core/ServiceLocator.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_entt()
		{
			// https://github.com/skaarj1989/entt-meets-sol2/blob/main/examples/registry/main.cpp
			// https://github.com/skaarj1989/entt-meets-sol2/blob/main/examples/dispatcher/main.cpp
			// https://github.com/skaarj1989/entt-meets-sol2/blob/main/examples/scheduler/main.cpp
			// https://github.com/skaarj1989/entt-meets-sol2/blob/main/examples/system/main.cpp
			// https://github.com/skaarj1989/entt-meets-sol2/tree/main/scripts

			auto& lua = core::ServiceLocator<sol::state>::ref();

			lua.require("registry", sol::c_call<AUTO_ARG(&entt_sol::open_registry)>);
			lua.require("dispatcher", sol::c_call<AUTO_ARG(&entt_sol::open_dispatcher)>);
			lua.require("scheduler", sol::c_call<AUTO_ARG(&entt_sol::open_scheduler)>);

			auto entt_anytype     = lua.new_usertype<entt::any>("EnttAny", sol::no_constructor);
			entt_anytype["reset"] = &entt::any::reset;
			entt_anytype["type"]  = &entt::any::type;
		}
	} // namespace lua
} // namespace galaxy
