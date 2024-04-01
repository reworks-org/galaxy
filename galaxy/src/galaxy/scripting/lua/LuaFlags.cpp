///
/// LuaFlags.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt_sol/registry.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/flags/DenySerialization.hpp"
#include "galaxy/flags/Disabled.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_flags()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			lua.new_usertype<flags::DenySerialization>("DenySerialization",
				sol::constructors<flags::DenySerialization()>(),
				"type_id",
				&entt::type_hash<flags::DenySerialization>::value);

			lua.new_usertype<flags::Disabled>("Disabled", sol::constructors<flags::Disabled()>(), "type_id", &entt::type_hash<flags::Disabled>::value);

			entt_sol::register_meta_component<flags::DenySerialization>();
			entt_sol::register_meta_component<flags::Disabled>();
		}
	} // namespace lua
} // namespace galaxy
