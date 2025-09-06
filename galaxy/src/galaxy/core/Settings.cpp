///
/// Settings.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>

#include "galaxy/core/Config.hpp"
#include "galaxy/platform/Pragma.hpp"

#include "Settings.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26860)
#endif

namespace galaxy
{
	auto Settings::set_config_to_default() -> void
	{
		auto& config = entt::locator<Config>::value();

		config.restore<int>("width", 1920, "window");
		config.restore<int>("height", 1080, "window");
		config.restore<std::string>("icon", "", "window");
		config.restore<bool>("fullscreen", false, "window");
		config.restore<bool>("maximized", true, "window");
		config.restore<bool>("vsync", false, "window");
		config.restore<bool>("mouse_grabbed", false, "window");
		config.restore<bool>("resizable", true, "window");
		config.restore<bool>("border", true, "window");

		config.restore<int>("audio_freq", 44100, "audio");

		config.restore<std::string>("title", "galaxy app", "meta");
		config.restore<std::string>("version", "1.0", "meta");
		config.restore<std::string>("identifier", "com.galaxy.app", "meta");
		config.restore<std::string>("creator", "reworks", "meta");
		config.restore<std::string>("copyright", "2025+ reworks", "meta");
		config.restore<std::string>("website", "https://reworks-org.github.io/galaxy/", "meta");

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

	auto Settings::set_settings_from_config() -> void
	{
		auto& config = entt::locator<Config>::value();

		s_window_width  = config.get<int>("width", "window").value();
		s_window_height = config.get<int>("height", "window").value();
		s_window_icon   = config.get<std::string>("icon", "window").value();
		s_fullscreen    = config.get<bool>("fullscreen", "window").value();
		s_maximized     = config.get<bool>("maximized", "window").value();
		s_vsync         = config.get<bool>("vsync", "window").value();
		s_mouse_grabbed = config.get<bool>("mouse_grabbed", "window").value();
		s_resizable     = config.get<bool>("resizable", "window").value();
		s_border        = config.get<bool>("border", "window").value();

		s_audio_freq = config.get<int>("audio_freq", "audio").value();

		s_title      = config.get<std::string>("title", "meta").value();
		s_version    = config.get<std::string>("version", "meta").value();
		s_identifier = config.get<std::string>("identifier", "meta").value();
		s_creator    = config.get<std::string>("creator", "meta").value();
		s_copyright  = config.get<std::string>("copyright", "meta").value();
		s_website    = config.get<std::string>("website", "meta").value();

		s_assets_dir       = config.get<std::string>("assets_dir", "fs").value();
		s_editor_dir       = config.get<std::string>("editor_dir", "fs").value();
		s_asset_pack       = config.get<std::string>("asset_pack", "fs").value();
		s_use_loose_assets = config.get<bool>("use_loose_assets", "fs").value();
		s_assets_music     = config.get<std::string>("assets_music", "fs").value();
		s_assets_sfx       = config.get<std::string>("assets_sfx", "fs").value();
		s_assets_voice     = config.get<std::string>("assets_voice", "fs").value();
		s_assets_font      = config.get<std::string>("assets_font", "fs").value();
		s_assets_script    = config.get<std::string>("assets_script", "fs").value();
		s_assets_shaders   = config.get<std::string>("assets_shaders", "fs").value();
		s_assets_animation = config.get<std::string>("assets_animation", "fs").value();
		s_assets_texture   = config.get<std::string>("assets_texture", "fs").value();
		s_assets_prefabs   = config.get<std::string>("assets_prefabs", "fs").value();
		s_assets_maps      = config.get<std::string>("assets_maps", "fs").value();
		s_assets_video     = config.get<std::string>("assets_video", "fs").value();
		s_assets_ui        = config.get<std::string>("assets_ui", "fs").value();
	}

	auto Settings::window_width() noexcept -> int
	{
		return s_window_width;
	}

	auto Settings::window_height() noexcept -> int
	{
		return s_window_height;
	}

	auto Settings::window_icon() noexcept -> const std::string&
	{
		return s_window_icon;
	}

	auto Settings::fullscreen() noexcept -> bool
	{
		return s_fullscreen;
	}

	auto Settings::maximized() noexcept -> bool
	{
		return s_maximized;
	}

	auto Settings::vsync() noexcept -> bool
	{
		return s_vsync;
	}

	auto Settings::mouse_grabbed() noexcept -> bool
	{
		return s_mouse_grabbed;
	}

	auto Settings::window_resizable() noexcept -> bool
	{
		return s_resizable;
	}

	auto Settings::window_border() noexcept -> bool
	{
		return s_border;
	}

	auto Settings::audio_freq() noexcept -> int
	{
		return s_audio_freq;
	}

	auto Settings::title() noexcept -> const std::string&
	{
		return s_title;
	}

	auto Settings::version() noexcept -> const std::string&
	{
		return s_version;
	}

	auto Settings::identifier() noexcept -> const std::string&
	{
		return s_identifier;
	}

	auto Settings::creator() noexcept -> const std::string&
	{
		return s_creator;
	}

	auto Settings::copyright() noexcept -> const std::string&
	{
		return s_copyright;
	}

	auto Settings::website() noexcept -> const std::string&
	{
		return s_website;
	}

	auto Settings::log_dir() noexcept -> const std::string&
	{
		static std::string log_dir = "logs/";
		return log_dir;
	}

	auto Settings::root_dir() noexcept -> std::filesystem::path
	{
		return std::filesystem::current_path();
	}

	auto Settings::assets_dir() noexcept -> std::filesystem::path
	{
		return s_assets_dir;
	}

	auto Settings::editor_dir() noexcept -> std::filesystem::path
	{
		return s_editor_dir;
	}

	auto Settings::asset_pack() noexcept -> const std::string&
	{
		return s_asset_pack;
	}

	auto Settings::use_loose_assets() noexcept -> bool
	{
		return s_use_loose_assets;
	}

	auto Settings::assets_dir_music() noexcept -> const std::string&
	{
		return s_assets_music;
	}

	auto Settings::assets_dir_sfx() noexcept -> const std::string&
	{
		return s_assets_sfx;
	}

	auto Settings::assets_dir_voice() noexcept -> const std::string&
	{
		return s_assets_voice;
	}

	auto Settings::assets_dir_font() noexcept -> const std::string&
	{
		return s_assets_font;
	}

	auto Settings::assets_dir_script() noexcept -> const std::string&
	{
		return s_assets_script;
	}

	auto Settings::assets_dir_shaders() noexcept -> const std::string&
	{
		return s_assets_shaders;
	}

	auto Settings::assets_dir_animation() noexcept -> const std::string&
	{
		return s_assets_animation;
	}

	auto Settings::assets_dir_texture() noexcept -> const std::string&
	{
		return s_assets_texture;
	}

	auto Settings::assets_dir_prefabs() noexcept -> const std::string&
	{
		return s_assets_prefabs;
	}

	auto Settings::assets_dir_maps() noexcept -> const std::string&
	{
		return s_assets_maps;
	}

	auto Settings::assets_dir_video() noexcept -> const std::string&
	{
		return s_assets_video;
	}

	auto Settings::assets_dir_ui() noexcept -> const std::string&
	{
		return s_assets_ui;
	}
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif
