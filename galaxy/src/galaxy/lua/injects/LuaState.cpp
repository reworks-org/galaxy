///
/// LuaStates.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/state/State.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	void Lua::inject_state() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();

		auto state_type       = lua.new_usertype<State>("State", sol::no_constructor);
		state_type["on_pop"]  = &State::on_pop;
		state_type["on_push"] = &State::on_push;
		state_type["name"]    = &State::name;
	}
} // namespace galaxy
