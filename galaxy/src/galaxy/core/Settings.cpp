///
/// Settings.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Settings.hpp"

namespace galaxy
{
	auto settings::set_config_to_default(core::Config& config) -> void
	{
		config.restore<double>("ups", 60.0f, "physics");

		config.restore<int>("width", 1920, "window");
		config.restore<int>("height", 1080, "window");
		config.restore<std::string>("title", "galaxy", "window");
		config.restore<std::string>("icon", "", "window");
		config.restore<bool>("fullscreen", false, "window");
		config.restore<bool>("key_repeat", true, "window");
		config.restore<bool>("vsync", false, "window");
		config.restore<bool>("msaa", false, "window");

		config.restore<bool>("visible", true, "cursor");
		config.restore<bool>("grab", false, "cursor");
		config.restore<std::string>("icon", "", "cursor");
		config.restore<int>("x", 0, "cursor.icon_size");
		config.restore<int>("y", 0, "cursor.icon_size");
		config.restore<int>("x", 0, "cursor.hotspot");
		config.restore<int>("y", 0, "cursor.hotspot");

		config.restore<std::string>("assets_dir", "assets/", "fs");
		config.restore<std::string>("editor_dir", "editor/", "fs");
		config.restore<std::string>("asset_pack", "assets.galaxy", "fs");
		config.restore<bool>("use_loose_assets", true, "fs");
		config.restore<std::string>("assets_music", "music/", "fs");
		config.restore<std::string>("assets_sfx", "sfx/", "fs");
		config.restore<std::string>("assets_voice", "voice/", "fs");
		config.restore<std::string>("assets_font", "fonts/", "fs");
		config.restore<std::string>("assets_script", "scripts/", "fs");
		config.restore<std::string>("assets_shaders", "shaders/", "fs");
		config.restore<std::string>("assets_animation", "animations/", "fs");
		config.restore<std::string>("assets_texture", "textures/", "fs");
		config.restore<std::string>("assets_prefabs", "prefabs/", "fs");
		config.restore<std::string>("assets_maps", "maps/", "fs");
		config.restore<std::string>("assets_video", "video/", "fs");
		config.restore<std::string>("assets_ui", "ui/", "fs");

		config.save();
	}

	auto settings::set_settings_from_config(core::Config& config) -> void
	{
		s_ups = config.get<double>("ups", "physics");

		s_window_width  = config.get<int>("width", "window");
		s_window_height = config.get<int>("height", "window");
		s_title         = config.get<std::string>("title", "window");
		s_icon          = config.get<std::string>("icon", "window");
		s_fullscreen    = config.get<bool>("fullscreen", "window");
		s_key_repeat    = config.get<bool>("key_repeat", "window");
		s_vsync         = config.get<bool>("vsync", "window");
		s_msaa          = config.get<bool>("msaa", "window");

		s_cursor_visible     = config.get<bool>("visible", "cursor");
		s_cursor_grabbed     = config.get<bool>("grab", "cursor");
		s_cursor_icon        = config.get<std::string>("icon", "cursor");
		s_cursor_icon_size.x = config.get<int>("x", "cursor.icon_size");
		s_cursor_icon_size.y = config.get<int>("y", "cursor.icon_size");
		s_cursor_hotspot.x   = config.get<int>("x", "cursor.hotspot");
		s_cursor_hotspot.y   = config.get<int>("y", "cursor.hotspot");

		s_assets_dir       = config.get<std::string>("assets_dir", "fs");
		s_editor_dir       = config.get<std::string>("editor_dir", "fs");
		s_asset_pack       = config.get<std::string>("asset_pack", "fs");
		s_use_loose_assets = config.get<bool>("use_loose_assets", "fs");
		s_assets_music     = config.get<std::string>("assets_music", "fs");
		s_assets_sfx       = config.get<std::string>("assets_sfx", "fs");
		s_assets_voice     = config.get<std::string>("assets_voice", "fs");
		s_assets_font      = config.get<std::string>("assets_font", "fs");
		s_assets_script    = config.get<std::string>("assets_script", "fs");
		s_assets_shaders   = config.get<std::string>("assets_shaders", "fs");
		s_assets_animation = config.get<std::string>("assets_animation", "fs");
		s_assets_texture   = config.get<std::string>("assets_texture", "fs");
		s_assets_prefabs   = config.get<std::string>("assets_prefabs", "fs");
		s_assets_maps      = config.get<std::string>("assets_maps", "fs");
		s_assets_video     = config.get<std::string>("assets_video", "fs");
		s_assets_ui        = config.get<std::string>("assets_ui", "fs");
	}

	auto settings::set_delta_time(const double dt) noexcept -> void
	{
		s_delta_time = dt;
	}

	auto settings::dt() noexcept -> double
	{
		return s_delta_time;
	}

	auto settings::ups() noexcept -> double
	{
		return s_ups;
	}

	auto settings::window_width() noexcept -> int
	{
		return s_window_width;
	}

	auto settings::window_height() noexcept -> int
	{
		return s_window_height;
	}

	auto settings::window_title() noexcept -> const std::string&
	{
		return s_title;
	}

	auto settings::window_icon() noexcept -> const std::string&
	{
		return s_icon;
	}

	auto settings::fullscreen() noexcept -> bool
	{
		return s_fullscreen;
	}

	auto settings::key_repeat() noexcept -> bool
	{
		return s_key_repeat;
	}

	auto settings::vsync() noexcept -> bool
	{
		return s_vsync;
	}

	auto settings::msaa() noexcept -> bool
	{
		return s_msaa;
	}

	auto settings::cursor_visible() noexcept -> bool
	{
		return s_cursor_visible;
	}

	auto settings::cursor_grabbed() noexcept -> bool
	{
		return s_cursor_grabbed;
	}

	auto settings::cursor_icon() noexcept -> const std::string&
	{
		return s_cursor_icon;
	}

	auto settings::cursor_icon_size() noexcept -> const sf::Vector2u&
	{
		return s_cursor_icon_size;
	}

	auto settings::cursor_hotspot() noexcept -> const sf::Vector2u&
	{
		return s_cursor_hotspot;
	}

	auto settings::root_dir() noexcept -> std::filesystem::path
	{
		return std::filesystem::current_path();
	}

	auto settings::assets_dir() noexcept -> std::filesystem::path
	{
		return s_assets_dir;
	}

	auto settings::editor_dir() noexcept -> std::filesystem::path
	{
		return s_editor_dir;
	}

	auto settings::asset_pack() noexcept -> const std::string&
	{
		return s_asset_pack;
	}

	auto settings::use_loose_assets() noexcept -> bool
	{
		return s_use_loose_assets;
	}

	auto settings::assets_dir_music() noexcept -> const std::string&
	{
		return s_assets_music;
	}

	auto settings::assets_dir_sfx() noexcept -> const std::string&
	{
		return s_assets_sfx;
	}

	auto settings::assets_dir_voice() noexcept -> const std::string&
	{
		return s_assets_voice;
	}

	auto settings::assets_dir_font() noexcept -> const std::string&
	{
		return s_assets_font;
	}

	auto settings::assets_dir_script() noexcept -> const std::string&
	{
		return s_assets_script;
	}

	auto settings::assets_dir_shaders() noexcept -> const std::string&
	{
		return s_assets_shaders;
	}

	auto settings::assets_dir_animation() noexcept -> const std::string&
	{
		return s_assets_animation;
	}

	auto settings::assets_dir_texture() noexcept -> const std::string&
	{
		return s_assets_texture;
	}

	auto settings::assets_dir_prefabs() noexcept -> const std::string&
	{
		return s_assets_prefabs;
	}

	auto settings::assets_dir_maps() noexcept -> const std::string&
	{
		return s_assets_maps;
	}

	auto settings::assets_dir_video() noexcept -> const std::string&
	{
		return s_assets_video;
	}

	auto settings::assets_dir_ui() noexcept -> const std::string&
	{
		return s_assets_ui;
	}
} // namespace galaxy
