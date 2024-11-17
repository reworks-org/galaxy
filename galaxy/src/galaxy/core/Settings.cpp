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
		config.restore<int>("flag_bitset_count", 8, "misc");

		config.restore<float>("world_to_box2d", 0.01f, "physics");
		config.restore<float>("box2d_to_world", 100.0f, "physics");
		config.restore<double>("ups", 60.0f, "physics");

		config.restore<int>("width", 1920, "window");
		config.restore<int>("height", 1080, "window");
		config.restore<std::string>("title", "galaxy", "window");
		config.restore<int>("screenmode", 1, "window");
		config.restore<bool>("vsync", false, "window");
		config.restore<bool>("mouse_visible", false, "window");
		config.restore<bool>("msaa", false, "window");
		config.restore<bool>("highdpi", false, "window");
		config.restore<std::string>("icon", "", "window");
		config.restore<std::string>("cursor_icon", "", "window");

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

	void settings::set_settings_from_config(core::Config& config)
	{
		s_flag_bitset_count = config.get<int>("flag_bitset_count", "misc");

		s_world_to_box2d = config.get<float>("world_to_box2d", "physics");
		s_box2d_to_world = config.get<float>("box2d_to_world", "physics");
		s_ups            = config.get<double>("ups", "physics");

		s_window_width  = config.get<int>("width", "window");
		s_window_height = config.get<int>("height", "window");
		s_title         = config.get<std::string>("title", "window");
		s_screenmode    = static_cast<graphics::ScreenMode>(config.get<int>("screenmode", "window"));
		s_vsync         = config.get<bool>("vsync", "window");
		s_mouse_visible = config.get<bool>("mouse_visible", "window");
		s_msaa          = config.get<bool>("msaa", "window");
		s_highdpi       = config.get<bool>("highdpi", "window");
		s_icon          = config.get<std::string>("icon", "window");
		s_cursor_icon   = config.get<std::string>("cursor_icon", "window");

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

	auto settings::set_delta_time(const double dt) -> void
	{
		s_delta_time = dt;
	}

	auto settings::dt() -> double
	{
		return s_delta_time;
	}

	auto settings::flag_bitset_count() -> int
	{
		return s_flag_bitset_count;
	}

	auto settings::exit_success() -> int
	{
		return 0;
	}

	auto settings::exit_failure() -> int
	{
		return 1;
	}

	auto settings::world_to_box2d() -> float
	{
		return s_world_to_box2d;
	}

	auto settings::box2d_to_world() -> float
	{
		return s_box2d_to_world;
	}

	auto settings::window_width() -> int
	{
		return s_window_width;
	}

	auto settings::window_height() -> int
	{
		return s_window_height;
	}

	auto settings::window_title() -> const std::string&
	{
		return s_title;
	}

	auto settings::screenmode() -> graphics::ScreenMode
	{
		return s_screenmode;
	}

	auto settings::vsync() -> bool
	{
		return s_vsync;
	}

	auto settings::mouse_visible() -> bool
	{
		return s_mouse_visible;
	}

	auto settings::msaa() -> bool
	{
		return s_msaa;
	}

	auto settings::highdpi() -> bool
	{
		return s_highdpi;
	}

	auto settings::window_icon() -> const std::string&
	{
		return s_icon;
	}

	auto settings::cursor_icon() -> const std::string&
	{
		return s_cursor_icon;
	}

	auto settings::ups() -> double
	{
		return s_ups;
	}

	auto settings::root_dir() -> std::filesystem::path
	{
		return std::filesystem::current_path();
	}

	auto settings::assets_dir() -> std::filesystem::path
	{
		return s_assets_dir;
	}

	auto settings::editor_dir() -> std::filesystem::path
	{
		return s_editor_dir;
	}

	auto settings::asset_pack() -> const std::string&
	{
		return s_asset_pack;
	}

	auto settings::use_loose_assets() -> bool
	{
		return s_use_loose_assets;
	}

	auto settings::assets_dir_music() -> const std::string&
	{
		return s_assets_music;
	}

	auto settings::assets_dir_sfx() -> const std::string&
	{
		return s_assets_sfx;
	}

	auto settings::assets_dir_voice() -> const std::string&
	{
		return s_assets_voice;
	}

	auto settings::assets_dir_font() -> const std::string&
	{
		return s_assets_font;
	}

	auto settings::assets_dir_script() -> const std::string&
	{
		return s_assets_script;
	}

	auto settings::assets_dir_shaders() -> const std::string&
	{
		return s_assets_shaders;
	}

	auto settings::assets_dir_animation() -> const std::string&
	{
		return s_assets_animation;
	}

	auto settings::assets_dir_texture() -> const std::string&
	{
		return s_assets_texture;
	}

	auto settings::assets_dir_prefabs() -> const std::string&
	{
		return s_assets_prefabs;
	}

	auto settings::assets_dir_maps() -> const std::string&
	{
		return s_assets_maps;
	}

	auto settings::assets_dir_video() -> const std::string&
	{
		return s_assets_video;
	}

	auto settings::assets_dir_ui() -> const std::string&
	{
		return s_assets_ui;
	}
} // namespace galaxy
