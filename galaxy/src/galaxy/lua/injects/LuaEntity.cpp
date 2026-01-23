///
/// LuaCore.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/entity/EntityManager.hpp"
#include "galaxy/entity/Registry.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	void Lua::inject_entity() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();

		auto registry_type    = lua.new_usertype<Registry>("Registry", sol::constructors<Registry()>());
		registry_type["entt"] = &Registry::m_entt;

		auto em_type        = lua.new_usertype<EntityManager>("EntityManager", sol::no_constructor);
		em_type["registry"] = &EntityManager::registry;

		/*auto prefab_type           = lua.new_usertype<Prefab>("Prefab", sol::constructors<Prefab(entt::entity, entt::registry&), Prefab(const nlohmann::json&)>());
		prefab_type["from_entity"] = &Prefab::from_entity;
		prefab_type["from_json"]   = &Prefab::from_json;
		prefab_type["to_entity"]   = &Prefab::to_entity;
		prefab_type["to_json"]     = &Prefab::to_json;
		prefab_type["load"]        = &Prefab::load;
		lua.set_function("galaxy_load_user_config", &load_config_wrapper);
		lua.set_function("galaxy_load_window_config", &load_window_wrapper);
		*/
	}
} // namespace galaxy
