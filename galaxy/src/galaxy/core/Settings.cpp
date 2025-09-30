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
GALAXY_DISABLE_WARNING(26830)
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
		config.restore<bool>("resizable", true, "window");
		config.restore<bool>("border", true, "window");

		config.restore<bool>("mouse_grabbed", false, "mouse");
		config.restore<bool>("cursor_show", true, "mouse");
		config.restore<std::string>("cursor_icon", "", "mouse");
		config.restore<int>("x", 0, "mouse.cursor_hotspot");
		config.restore<int>("y", 0, "mouse.cursor_hotspot");

		config.restore<int>("audio_freq", 44100, "audio");

		config.restore<int>("ansiotrophy", 16, "graphics");
		config.restore<bool>("mipmap", true, "graphics");
		config.restore<std::string>("texture_filter", "NEAREST", "graphics");

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

		s_window_width  = *config.get<int>("width", "window");
		s_window_height = *config.get<int>("height", "window");
		s_window_icon   = *config.get<std::string>("icon", "window");
		s_fullscreen    = *config.get<bool>("fullscreen", "window");
		s_maximized     = *config.get<bool>("maximized", "window");
		s_vsync         = *config.get<bool>("vsync", "window");
		s_resizable     = *config.get<bool>("resizable", "window");
		s_border        = *config.get<bool>("border", "window");

		s_mouse_grabbed    = *config.get<bool>("mouse_grabbed", "mouse");
		s_cursor_show      = *config.get<bool>("cursor_show", "mouse");
		s_cursor_icon      = *config.get<std::string>("cursor_icon", "mouse");
		s_cursor_hotspot.x = *config.get<int>("x", "mouse.cursor_hotspot");
		s_cursor_hotspot.y = *config.get<int>("y", "mouse.cursor_hotspot");

		s_audio_freq = *config.get<int>("audio_freq", "audio");

		s_ansio     = *config.get<int>("ansiotrophy", "graphics");
		s_mipmap    = *config.get<bool>("mipmap", "graphics");
		s_filtering = *config.get<std::string>("texture_filter", "graphics").and_then([](std::string_view filter) {
			return magic_enum::enum_cast<GLTextureFilter>(filter);
		});

		s_title      = *config.get<std::string>("title", "meta");
		s_version    = *config.get<std::string>("version", "meta");
		s_identifier = *config.get<std::string>("identifier", "meta");
		s_creator    = *config.get<std::string>("creator", "meta");
		s_copyright  = *config.get<std::string>("copyright", "meta");
		s_website    = *config.get<std::string>("website", "meta");

		s_assets_dir       = *config.get<std::string>("assets_dir", "fs");
		s_editor_dir       = *config.get<std::string>("editor_dir", "fs");
		s_asset_pack       = *config.get<std::string>("asset_pack", "fs");
		s_use_loose_assets = *config.get<bool>("use_loose_assets", "fs");
		s_assets_music     = *config.get<std::string>("assets_music", "fs");
		s_assets_sfx       = *config.get<std::string>("assets_sfx", "fs");
		s_assets_voice     = *config.get<std::string>("assets_voice", "fs");
		s_assets_font      = *config.get<std::string>("assets_font", "fs");
		s_assets_script    = *config.get<std::string>("assets_script", "fs");
		s_assets_shaders   = *config.get<std::string>("assets_shaders", "fs");
		s_assets_animation = *config.get<std::string>("assets_animation", "fs");
		s_assets_texture   = *config.get<std::string>("assets_texture", "fs");
		s_assets_prefabs   = *config.get<std::string>("assets_prefabs", "fs");
		s_assets_maps      = *config.get<std::string>("assets_maps", "fs");
		s_assets_video     = *config.get<std::string>("assets_video", "fs");
		s_assets_ui        = *config.get<std::string>("assets_ui", "fs");
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

	auto Settings::window_resizable() noexcept -> bool
	{
		return s_resizable;
	}

	auto Settings::window_border() noexcept -> bool
	{
		return s_border;
	}

	auto Settings::mouse_grabbed() noexcept -> bool
	{
		return s_mouse_grabbed;
	}

	auto Settings::cursor_show() noexcept -> bool
	{
		return s_cursor_show;
	}

	auto Settings::cursor_icon() noexcept -> const std::string&
	{
		return s_cursor_icon;
	}

	auto Settings::cursor_hotspot() noexcept -> const glm::ivec2&
	{
		return s_cursor_hotspot;
	}

	auto Settings::audio_freq() noexcept -> int
	{
		return s_audio_freq;
	}

	auto Settings::ansiotrophy() noexcept -> int
	{
		return s_ansio;
	}

	auto Settings::mipmap() noexcept -> bool
	{
		return s_mipmap;
	}

	auto Settings::texture_filter() noexcept -> GLTextureFilter
	{
		return s_filtering;
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
