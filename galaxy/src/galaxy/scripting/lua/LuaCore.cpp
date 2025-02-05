///
/// LuaCore.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/Loader.hpp"
#include "galaxy/core/Prefab.hpp"
#include "galaxy/core/Registry.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void load_config_wrapper()
		{
			core::ServiceLocator<core::Loader>::ref().load_user_config();
		}

		void load_window_wrapper()
		{
			core::ServiceLocator<core::Loader>::ref().load_window();
		}

		void inject_core()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			auto config_type                  = lua.new_usertype<core::Config>("Config", sol::no_constructor);
			config_type["save"]               = &core::Config::save;
			config_type["has_at_root"]        = sol::resolve<bool(const std::string&)>(&core::Config::has);
			config_type["has_in_section"]     = sol::resolve<bool(const std::string&, const std::string&, const std::string&)>(&core::Config::has);
			config_type["get_bool"]           = sol::resolve<bool(const std::string&)>(&core::Config::get<bool>);
			config_type["get_int"]            = sol::resolve<int(const std::string&)>(&core::Config::get<int>);
			config_type["get_float"]          = sol::resolve<float(const std::string&)>(&core::Config::get<float>);
			config_type["get_string"]         = sol::resolve<std::string(const std::string&)>(&core::Config::get<std::string>);
			config_type["get_section_bool"]   = sol::resolve<bool(const std::string&, const std::string&, const std::string&)>(&core::Config::get<bool>);
			config_type["get_section_int"]    = sol::resolve<int(const std::string&, const std::string&, const std::string&)>(&core::Config::get<int>);
			config_type["get_section_float"]  = sol::resolve<float(const std::string&, const std::string&, const std::string&)>(&core::Config::get<float>);
			config_type["get_section_string"] = sol::resolve<std::string(const std::string&, const std::string&, const std::string&)>(&core::Config::get<std::string>);

			lua.set_function("galaxy_load_user_config", &load_config_wrapper);
			lua.set_function("galaxy_load_window_config", &load_window_wrapper);

			auto registry_type                  = lua.new_usertype<core::Registry>("Registry", sol::constructors<core::Registry()>());
			registry_type["clear"]              = &core::Registry::clear;
			registry_type["create"]             = &core::Registry::create;
			registry_type["create_from_prefab"] = &core::Registry::create_from_prefab;
			registry_type["is_valid"]           = &core::Registry::is_valid;
			registry_type["entt"]               = &core::Registry::m_entt;

			auto prefab_type = lua.new_usertype<core::Prefab>("Prefab", sol::constructors<core::Prefab(entt::entity, entt::registry&), core::Prefab(const nlohmann::json&)>());
			prefab_type["from_entity"] = &core::Prefab::from_entity;
			prefab_type["from_json"]   = &core::Prefab::from_json;
			prefab_type["to_entity"]   = &core::Prefab::to_entity;
			prefab_type["to_json"]     = &core::Prefab::to_json;
			prefab_type["load"]        = &core::Prefab::load;
		}
	} // namespace lua
} // namespace galaxy
