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

	auto settings::set_settings_from_config(core::Config& config) -> void
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

	auto settings::set_delta_time(const double dt) noexcept -> void
	{
		s_delta_time = dt;
	}

	auto settings::dt() noexcept -> double
	{
		return s_delta_time;
	}

	auto settings::flag_bitset_count() noexcept -> int
	{
		return s_flag_bitset_count;
	}

	auto settings::exit_success() noexcept -> int
	{
		return 0;
	}

	auto settings::exit_failure() noexcept -> int
	{
		return 1;
	}

	auto settings::world_to_box2d() noexcept -> float
	{
		return s_world_to_box2d;
	}

	auto settings::box2d_to_world() noexcept -> float
	{
		return s_box2d_to_world;
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

	auto settings::screenmode() noexcept -> graphics::ScreenMode
	{
		return s_screenmode;
	}

	auto settings::vsync() noexcept -> bool
	{
		return s_vsync;
	}

	auto settings::mouse_visible() noexcept -> bool
	{
		return s_mouse_visible;
	}

	auto settings::msaa() noexcept -> bool
	{
		return s_msaa;
	}

	auto settings::highdpi() noexcept -> bool
	{
		return s_highdpi;
	}

	auto settings::window_icon() noexcept -> const std::string&
	{
		return s_icon;
	}

	auto settings::cursor_icon() noexcept -> const std::string&
	{
		return s_cursor_icon;
	}

	auto settings::ups() noexcept -> double
	{
		return s_ups;
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
