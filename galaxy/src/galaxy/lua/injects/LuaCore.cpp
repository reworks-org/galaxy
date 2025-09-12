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
#include "galaxy/core/Window.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	std::string root_path_wrapper() noexcept
	{
		return Settings::root_dir().string();
	}

	std::string assets_path_wrapper() noexcept
	{
		return Settings::assets_dir().string();
	}

	std::string editor_path_wrapper() noexcept
	{
		return Settings::editor_dir().string();
	}

	void Lua::inject_core() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();

		auto config_type                      = lua.new_usertype<Config>("Config", sol::no_constructor);
		config_type["load"]                   = &Config::load;
		config_type["save"]                   = &Config::save;
		config_type["set_bool"]               = sol::resolve<void(const std::string&, const bool&)>(&Config::set<bool>);
		config_type["set_int"]                = sol::resolve<void(const std::string&, const int&)>(&Config::set<int>);
		config_type["set_float"]              = sol::resolve<void(const std::string&, const float&)>(&Config::set<float>);
		config_type["set_string"]             = sol::resolve<void(const std::string&, const std::string&)>(&Config::set<std::string>);
		config_type["set_section_bool"]       = sol::resolve<void(const std::string&, const bool&, const std::string&, const std::string&)>(&Config::set<bool>);
		config_type["set_section_int"]        = sol::resolve<void(const std::string&, const int&, const std::string&, const std::string&)>(&Config::set<int>);
		config_type["set_section_float"]      = sol::resolve<void(const std::string&, const float&, const std::string&, const std::string&)>(&Config::set<float>);
		config_type["set_section_string"]     = sol::resolve<void(const std::string&, const std::string&, const std::string&, const std::string&)>(&Config::set<std::string>);
		config_type["restore_bool"]           = sol::resolve<void(const std::string&, const bool&)>(&Config::restore<bool>);
		config_type["restore_int"]            = sol::resolve<void(const std::string&, const int&)>(&Config::restore<int>);
		config_type["restore_float"]          = sol::resolve<void(const std::string&, const float&)>(&Config::restore<float>);
		config_type["restore_string"]         = sol::resolve<void(const std::string&, const std::string&)>(&Config::restore<std::string>);
		config_type["restore_section_bool"]   = sol::resolve<void(const std::string&, const bool&, const std::string&, const std::string&)>(&Config::restore<bool>);
		config_type["restore_section_int"]    = sol::resolve<void(const std::string&, const int&, const std::string&, const std::string&)>(&Config::restore<int>);
		config_type["restore_section_float"]  = sol::resolve<void(const std::string&, const float&, const std::string&, const std::string&)>(&Config::restore<float>);
		config_type["restore_section_string"] = sol::resolve<void(const std::string&, const std::string&, const std::string&, const std::string&)>(&Config::restore<std::string>);
		config_type["has"]                    = sol::resolve<bool(const std::string&)>(&Config::has);
		config_type["has_section"]            = sol::resolve<bool(const std::string&, const std::string&, const std::string&)>(&Config::has);
		config_type["get_bool"]               = sol::resolve<std::optional<bool>(const std::string&)>(&Config::get<bool>);
		config_type["get_int"]                = sol::resolve<std::optional<int>(const std::string&)>(&Config::get<int>);
		config_type["get_float"]              = sol::resolve<std::optional<float>(const std::string&)>(&Config::get<float>);
		config_type["get_string"]             = sol::resolve<std::optional<std::string>(const std::string&)>(&Config::get<std::string>);
		config_type["get_section_int"]        = sol::resolve<std::optional<int>(const std::string&, const std::string&, const std::string&)>(&Config::get<int>);
		config_type["get_section_float"]      = sol::resolve<std::optional<float>(const std::string&, const std::string&, const std::string&)>(&Config::get<float>);
		config_type["get_section_string"]     = sol::resolve<std::optional<std::string>(const std::string&, const std::string&, const std::string&)>(&Config::get<std::string>);
		config_type["get_section_bool"]       = sol::resolve<std::optional<bool>(const std::string&, const std::string&, const std::string&)>(&Config::get<bool>);
		config_type["empty"]                  = &Config::empty;

		lua.set_function("settings_set_to_default", &Settings::set_config_to_default);
		lua.set_function("settings_set_from_config", &Settings::set_settings_from_config);
		lua.set_function("settings_window_width", &Settings::window_width);
		lua.set_function("settings_window_height", &Settings::window_height);
		lua.set_function("settings_window_icon", &Settings::window_icon);
		lua.set_function("settings_fullscreen", &Settings::fullscreen);
		lua.set_function("settings_maximized", &Settings::maximized);
		lua.set_function("settings_vsync", &Settings::vsync);
		lua.set_function("settings_window_resizable", &Settings::window_resizable);
		lua.set_function("settings_window_border", &Settings::window_border);
		lua.set_function("settings_mouse_grabbed", &Settings::mouse_grabbed);
		lua.set_function("settings_cursor_show", &Settings::cursor_show);
		lua.set_function("settings_cursor_icon", &Settings::cursor_icon);
		lua.set_function("settings_cursor_hotspot", &Settings::cursor_hotspot);
		lua.set_function("settings_audio_freq", &Settings::audio_freq);
		lua.set_function("settings_title", &Settings::title);
		lua.set_function("settings_version", &Settings::version);
		lua.set_function("settings_identifier", &Settings::identifier);
		lua.set_function("settings_creator", &Settings::creator);
		lua.set_function("settings_copyright", &Settings::copyright);
		lua.set_function("settings_website", &Settings::website);
		lua.set_function("settings_log_dir", &Settings::log_dir);
		lua.set_function("settings_root_dir", &root_path_wrapper);
		lua.set_function("settings_assets_dir", &assets_path_wrapper);
		lua.set_function("settings_editor_dir", &editor_path_wrapper);
		lua.set_function("settings_asset_pack", &Settings::asset_pack);
		lua.set_function("settings_use_loose_assets", &Settings::use_loose_assets);
		lua.set_function("settings_assets_dir_music", &Settings::assets_dir_music);
		lua.set_function("settings_assets_dir_sfx", &Settings::assets_dir_sfx);
		lua.set_function("settings_assets_dir_voice", &Settings::assets_dir_voice);
		lua.set_function("settings_assets_dir_font", &Settings::assets_dir_font);
		lua.set_function("settings_assets_dir_script", &Settings::assets_dir_script);
		lua.set_function("settings_assets_dir_shaders", &Settings::assets_dir_shaders);
		lua.set_function("settings_assets_dir_animation", &Settings::assets_dir_animation);
		lua.set_function("settings_assets_dir_texture", &Settings::assets_dir_texture);
		lua.set_function("settings_assets_dir_prefabs", &Settings::assets_dir_prefabs);
		lua.set_function("settings_assets_dir_maps", &Settings::assets_dir_maps);
		lua.set_function("settings_assets_dir_video", &Settings::assets_dir_video);
		lua.set_function("settings_assets_dir_ui", &Settings::assets_dir_ui);

		auto window_type                    = lua.new_usertype<Window>("Window", sol::no_constructor);
		window_type["append_title"]         = &Window::append_title;
		window_type["close"]                = &Window::close;
		window_type["get_keyboard"]         = &Window::get_keyboard;
		window_type["get_mouse"]            = &Window::get_mouse;
		window_type["get_pixel_size"]       = &Window::get_pixel_size;
		window_type["hide"]                 = &Window::hide;
		window_type["is_open"]              = &Window::is_open;
		window_type["maximize"]             = &Window::maximize;
		window_type["minimize"]             = &Window::minimize;
		window_type["raise"]                = &Window::raise;
		window_type["request_attention"]    = &Window::request_attention;
		window_type["resize"]               = &Window::resize;
		window_type["restore"]              = &Window::restore;
		window_type["set_fullscreen"]       = &Window::set_fullscreen;
		window_type["set_icon"]             = &Window::set_icon;
		window_type["set_taskbar_progress"] = &Window::set_taskbar_progress;
		window_type["show"]                 = &Window::show;

		/*auto registry_type                  = lua.new_usertype<Registry>("Registry", sol::constructors<Registry()>());
		registry_type["clear"]              = &Registry::clear;
		registry_type["create"]             = &Registry::create;
		registry_type["create_from_prefab"] = &Registry::create_from_prefab;
		registry_type["is_valid"]           = &Registry::is_valid;
		registry_type["entt"]               = &Registry::m_entt;

		auto prefab_type = lua.new_usertype<Prefab>("Prefab", sol::constructors<Prefab(entt::entity, entt::registry&), Prefab(const nlohmann::json&)>());
		prefab_type["from_entity"] = &Prefab::from_entity;
		prefab_type["from_json"]   = &Prefab::from_json;
		prefab_type["to_entity"]   = &Prefab::to_entity;
		prefab_type["to_json"]     = &Prefab::to_json;
		prefab_type["load"]        = &Prefab::load;
		lua.set_function("galaxy_load_user_config", &load_config_wrapper);
		lua.set_function("galaxy_load_window_config", &load_window_wrapper);*/
	}
} // namespace galaxy
