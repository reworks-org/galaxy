///
/// LuaFlags.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <entt_sol/registry.hpp>

#include "galaxy/flags/Disabled.hpp"
#include "galaxy/flags/NotSerializable.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_flags()
		{
			auto& lua = entt::locator<sol::state>::value();

			lua.new_usertype<flags::NotSerializable>("NotSerializable", sol::constructors<flags::NotSerializable()>(), "type_id", &entt::type_hash<flags::NotSerializable>::value);
			lua.new_usertype<flags::Disabled>("Disabled", sol::constructors<flags::Disabled()>(), "type_id", &entt::type_hash<flags::Disabled>::value);

			entt_sol::register_meta_component<flags::NotSerializable>();
			entt_sol::register_meta_component<flags::Disabled>();
		}
	} // namespace lua
} // namespace galaxy
