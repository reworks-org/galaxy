///
/// LuaStates.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/state/StateMachine.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_states()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			auto state_type       = lua.new_usertype<state::State>("State", sol::no_constructor);
			state_type["on_pop"]  = &state::State::on_pop;
			state_type["on_push"] = &state::State::on_push;
			state_type["update"]  = &state::State::update;

			auto statemachine_type = lua.new_usertype<state::StateMachine>("StateMachine", sol::constructors<state::StateMachine()>());
			// statemachine_type[""]  = &state::StateMachine::add;
			statemachine_type["pop"]    = &state::StateMachine::pop;
			statemachine_type["push"]   = &state::StateMachine::push;
			statemachine_type["update"] = &state::StateMachine::update;
		}
	} // namespace lua
} // namespace galaxy
