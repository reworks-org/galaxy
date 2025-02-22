///
/// LuaCore.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/Settings.hpp"

namespace galaxy
{
	namespace lua
	{
		std::string root_path_wrapper() noexcept
		{
			return settings::root_dir().string();
		}

		std::string assets_path_wrapper() noexcept
		{
			return settings::assets_dir().string();
		}

		std::string editor_path_wrapper() noexcept
		{
			return settings::editor_dir().string();
		}

		/*void load_config_wrapper()
		{
			core::ServiceLocator<core::Loader>::ref().load_user_config();
		}

		void load_window_wrapper()
		{
			core::ServiceLocator<core::Loader>::ref().load_window();
		}*/

		void inject_core()
		{
			auto& lua = entt::locator<sol::state>::value();

			// lua.set_function("galaxy_load_user_config", &load_config_wrapper);
			// lua.set_function("galaxy_load_window_config", &load_window_wrapper);

			auto config_type                  = lua.new_usertype<core::Config>("Config", sol::no_constructor);
			config_type["save"]               = &core::Config::save;
			config_type["empty"]              = &core::Config::empty;
			config_type["has_at_root"]        = sol::resolve<bool(const std::string&)>(&core::Config::has);
			config_type["has_in_section"]     = sol::resolve<bool(const std::string&, const std::string&, const std::string&)>(&core::Config::has);
			config_type["set_bool"]           = sol::resolve<void(const std::string&, const bool&)>(&core::Config::set<bool>);
			config_type["set_int"]            = sol::resolve<void(const std::string&, const int&)>(&core::Config::set<int>);
			config_type["set_float"]          = sol::resolve<void(const std::string&, const float&)>(&core::Config::set<float>);
			config_type["set_string"]         = sol::resolve<void(const std::string&, const std::string&)>(&core::Config::set<std::string>);
			config_type["restore_bool"]       = sol::resolve<void(const std::string&, const bool&)>(&core::Config::restore<bool>);
			config_type["restore_int"]        = sol::resolve<void(const std::string&, const int&)>(&core::Config::restore<int>);
			config_type["restore_float"]      = sol::resolve<void(const std::string&, const float&)>(&core::Config::restore<float>);
			config_type["restore_string"]     = sol::resolve<void(const std::string&, const std::string&)>(&core::Config::restore<std::string>);
			config_type["get_bool"]           = sol::resolve<bool(const std::string&)>(&core::Config::get<bool>);
			config_type["get_int"]            = sol::resolve<int(const std::string&)>(&core::Config::get<int>);
			config_type["get_float"]          = sol::resolve<float(const std::string&)>(&core::Config::get<float>);
			config_type["get_string"]         = sol::resolve<std::string(const std::string&)>(&core::Config::get<std::string>);
			config_type["set_section_bool"]   = sol::resolve<void(const std::string&, const bool&, const std::string&, const std::string&)>(&core::Config::set<bool>);
			config_type["set_section_int"]    = sol::resolve<void(const std::string&, const int&, const std::string&, const std::string&)>(&core::Config::set<int>);
			config_type["set_section_float"]  = sol::resolve<void(const std::string&, const float&, const std::string&, const std::string&)>(&core::Config::set<float>);
			config_type["set_section_string"] = sol::resolve<void(const std::string&, const std::string&, const std::string&, const std::string&)>(&core::Config::set<std::string>);
			config_type["restore_section_bool"]  = sol::resolve<void(const std::string&, const bool&, const std::string&, const std::string&)>(&core::Config::restore<bool>);
			config_type["restore_section_int"]   = sol::resolve<void(const std::string&, const int&, const std::string&, const std::string&)>(&core::Config::restore<int>);
			config_type["restore_section_float"] = sol::resolve<void(const std::string&, const float&, const std::string&, const std::string&)>(&core::Config::restore<float>);
			config_type["restore_section_string"] =
				sol::resolve<void(const std::string&, const std::string&, const std::string&, const std::string&)>(&core::Config::restore<std::string>);
			config_type["get_section_bool"]   = sol::resolve<bool(const std::string&, const std::string&, const std::string&)>(&core::Config::get<bool>);
			config_type["get_section_int"]    = sol::resolve<int(const std::string&, const std::string&, const std::string&)>(&core::Config::get<int>);
			config_type["get_section_float"]  = sol::resolve<float(const std::string&, const std::string&, const std::string&)>(&core::Config::get<float>);
			config_type["get_section_string"] = sol::resolve<std::string(const std::string&, const std::string&, const std::string&)>(&core::Config::get<std::string>);

			//
			// SETTINGS FUNCTIONS.
			//

			lua.set("GALAXY_ZLIB_COMPLETE_CHUNK", GALAXY_ZLIB_COMPLETE_CHUNK);

			lua.set_function("settings_dt", &settings::dt);
			lua.set_function("settings_ups", &settings::ups);
			lua.set_function("settings_window_width", &settings::window_width);
			lua.set_function("settings_window_height", &settings::window_height);
			lua.set_function("settings_window_title", &settings::window_title);
			lua.set_function("settings_window_icon", &settings::window_icon);
			lua.set_function("settings_fullscreen", &settings::fullscreen);
			lua.set_function("settings_key_repeat", &settings::key_repeat);
			lua.set_function("settings_vsync", &settings::vsync);
			lua.set_function("settings_msaa", &settings::msaa);
			lua.set_function("settings_cursor_visible", &settings::cursor_visible);
			lua.set_function("settings_cursor_grabbed", &settings::cursor_grabbed);
			lua.set_function("settings_cursor_icon", &settings::cursor_icon);
			lua.set_function("settings_cursor_icon_size", &settings::cursor_icon_size);
			lua.set_function("settings_cursor_hotspot", &settings::cursor_hotspot);
			lua.set_function("settings_root_dir", &root_path_wrapper);
			lua.set_function("settings_assets_dir", &assets_path_wrapper);
			lua.set_function("settings_editor_dir", &editor_path_wrapper);
			lua.set_function("settings_asset_pack", &settings::asset_pack);
			lua.set_function("settings_use_loose_assets", &settings::use_loose_assets);
			lua.set_function("settings_assets_dir_music", &settings::assets_dir_music);
			lua.set_function("settings_assets_dir_sfx", &settings::assets_dir_sfx);
			lua.set_function("settings_assets_dir_voice", &settings::assets_dir_voice);
			lua.set_function("settings_assets_dir_font", &settings::assets_dir_font);
			lua.set_function("settings_assets_dir_script", &settings::assets_dir_script);
			lua.set_function("settings_assets_dir_shaders", &settings::assets_dir_shaders);
			lua.set_function("settings_assets_dir_animation", &settings::assets_dir_animation);
			lua.set_function("settings_assets_dir_texture", &settings::assets_dir_texture);
			lua.set_function("settings_assets_dir_prefabs", &settings::assets_dir_prefabs);
			lua.set_function("settings_assets_dir_maps", &settings::assets_dir_maps);
			lua.set_function("settings_assets_dir_video", &settings::assets_dir_video);
			lua.set_function("settings_assets_dir_ui", &settings::assets_dir_ui);

			/*auto registry_type                  = lua.new_usertype<core::Registry>("Registry", sol::constructors<core::Registry()>());
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
			prefab_type["load"]        = &core::Prefab::load;*/
		}
	} // namespace lua
} // namespace galaxy
