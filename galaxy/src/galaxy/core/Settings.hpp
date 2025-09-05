///
/// Settings.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_SETTINGS_HPP_
#define GALAXY_CORE_SETTINGS_HPP_

#include <filesystem>

namespace galaxy
{
	///
	/// Helper class to encapsulate our global settings we want to quickly access across app.
	///
	struct settings
	{
	public:
		///
		/// Restore all config settings to default.
		///
		static auto set_config_to_default() -> void;

		///
		/// Set all our settings using the provided config file.
		///
		static auto set_settings_from_config() -> void;

		///
		/// Window creation width.
		///
		/// \return Integer.
		///
		[[nodiscard]]
		static auto window_width() noexcept -> int;

		///
		/// Window creation height.
		///
		/// \return Integer.
		///
		[[nodiscard]]
		static auto window_height() noexcept -> int;

		///
		/// Window icon file in vfs.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto window_icon() noexcept -> const std::string&;

		///
		/// Is window started fullscreen.
		///
		/// \return Boolean.
		///
		[[nodiscard]]
		static auto fullscreen() noexcept -> bool;

		///
		/// \brief Is window started maximized?
		///
		/// Fullscreen takes priority.
		///
		/// \return Boolean.
		///
		[[nodiscard]]
		static auto maximized() noexcept -> bool;

		///
		/// Vsync control.
		///
		/// \return Boolean.
		///
		[[nodiscard]]
		static auto vsync() noexcept -> bool;

		///
		/// Is the cursor grabbed.
		///
		/// \return Boolean.
		///
		[[nodiscard]]
		static auto mouse_grabbed() noexcept -> bool;

		///
		/// Is the window resizable.
		///
		/// \return Boolean.
		///
		[[nodiscard]]
		static auto window_resizable() noexcept -> bool;

		///
		/// Controls if a window has a border around it (including titlebar).
		///
		/// \return Boolean.
		///
		[[nodiscard]]
		static auto window_border() noexcept -> bool;

		///
		/// Set audio frequency to use with SDL.
		///
		/// \return Int.
		///
		[[nodiscard]]
		static auto audio_freq() noexcept -> int;

		///
		/// Game title.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto title() noexcept -> const std::string&;

		///
		/// Game semver.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto version() noexcept -> const std::string&;

		///
		/// Game identifier i.e. com.galaxy.app.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto identifier() noexcept -> const std::string&;

		///
		/// Owner.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto creator() noexcept -> const std::string&;

		///
		/// Copyright message.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto copyright() noexcept -> const std::string&;

		///
		/// Website URL.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto website() noexcept -> const std::string&;

		///
		/// Current root directory of application, unless it has been changed.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto log_dir() noexcept -> const std::string&;

		///
		/// Current root directory of application, unless it has been changed.
		///
		/// \return Filesystem path.
		///
		[[nodiscard]]
		static auto root_dir() noexcept -> std::filesystem::path;

		///
		/// Main data directory.
		///
		/// \return Filesystem path.
		///
		[[nodiscard]]
		static auto assets_dir() noexcept -> std::filesystem::path;

		///
		/// Directory for editor specific stuff.
		///
		/// \return Filesystem path.
		///
		[[nodiscard]]
		static auto editor_dir() noexcept -> std::filesystem::path;

		///
		/// Name of packed assets file.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto asset_pack() noexcept -> const std::string&;

		///
		/// Should asset data be read from pack or assets dir.
		///
		/// \return Boolean.
		///
		[[nodiscard]]
		static auto use_loose_assets() noexcept -> bool;

		///
		/// Music asset location.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto assets_dir_music() noexcept -> const std::string&;

		///
		/// SFX asset location.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto assets_dir_sfx() noexcept -> const std::string&;

		///
		/// Voice asset location.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto assets_dir_voice() noexcept -> const std::string&;

		///
		/// Font asset location.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto assets_dir_font() noexcept -> const std::string&;

		///
		/// Scripts asset location.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto assets_dir_script() noexcept -> const std::string&;

		///
		/// Shaders asset location.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto assets_dir_shaders() noexcept -> const std::string&;

		///
		/// Animation data location.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto assets_dir_animation() noexcept -> const std::string&;

		///
		/// Textures asset location.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto assets_dir_texture() noexcept -> const std::string&;

		///
		/// Prefab asset location.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto assets_dir_prefabs() noexcept -> const std::string&;

		///
		/// Maps asset location.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto assets_dir_maps() noexcept -> const std::string&;

		///
		/// Video asset location.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto assets_dir_video() noexcept -> const std::string&;

		///
		/// UI asset location.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		static auto assets_dir_ui() noexcept -> const std::string&;

	private:
		inline static int         s_window_width;
		inline static int         s_window_height;
		inline static std::string s_window_icon;
		inline static bool        s_fullscreen;
		inline static bool        s_maximized;
		inline static bool        s_vsync;
		inline static bool        s_mouse_grabbed;
		inline static bool        s_resizable;
		inline static bool        s_border;

		inline static int s_audio_freq;

		inline static std::string s_title;
		inline static std::string s_version;
		inline static std::string s_identifier;
		inline static std::string s_creator;
		inline static std::string s_copyright;
		inline static std::string s_website;

		inline static std::filesystem::path s_assets_dir;
		inline static std::filesystem::path s_editor_dir;
		inline static std::string           s_asset_pack;
		inline static bool                  s_use_loose_assets;
		inline static std::string           s_assets_music;
		inline static std::string           s_assets_sfx;
		inline static std::string           s_assets_voice;
		inline static std::string           s_assets_font;
		inline static std::string           s_assets_script;
		inline static std::string           s_assets_shaders;
		inline static std::string           s_assets_animation;
		inline static std::string           s_assets_texture;
		inline static std::string           s_assets_prefabs;
		inline static std::string           s_assets_maps;
		inline static std::string           s_assets_video;
		inline static std::string           s_assets_ui;
	};
} // namespace galaxy

/*

auto settings::view_width() noexcept -> int
	{
		return s_view_width;
	}

	auto settings::view_height() noexcept -> int
	{
		return s_view_height;
	}

	auto settings::key_repeat() noexcept -> bool
	{
		return s_key_repeat;
	}


	auto settings::msaa() noexcept -> bool
	{
		return s_msaa;
	}

	auto settings::cursor_visible() noexcept -> bool
	{
		return s_cursor_visible;
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

		config.restore<int>("view_width", 1280, "window");
		config.restore<int>("view_height", 720, "window");
		config.restore<bool>("key_repeat", true, "window");
		config.restore<bool>("msaa", false, "window");
		config.restore<bool>("visible", true, "cursor");
		config.restore<std::string>("icon", "", "cursor");
		config.restore<int>("x", 0, "cursor.icon_size");
		config.restore<int>("y", 0, "cursor.icon_size");
		config.restore<int>("x", 0, "cursor.hotspot");
		config.restore<int>("y", 0, "cursor.hotspot");
// s_view_width    = config.get<int>("view_width", "window");
// s_view_height   = config.get<int>("view_height", "window");
// s_key_repeat    = config.get<bool>("key_repeat", "window");
// s_msaa          = config.get<bool>("msaa", "window");
// s_cursor_visible     = config.get<bool>("visible", "cursor");
// s_cursor_icon        = config.get<std::string>("icon", "cursor");
// s_cursor_icon_size.x = config.get<int>("x", "cursor.icon_size");
// s_cursor_icon_size.y = config.get<int>("y", "cursor.icon_size");
// s_cursor_hotspot.x   = config.get<int>("x", "cursor.hotspot");
// s_cursor_hotspot.y   = config.get<int>("y", "cursor.hotspot");

		inline static int         s_view_width;
		inline static int         s_view_height;
		inline static bool        s_key_repeat;
		inline static bool        s_msaa;

		inline static bool         s_cursor_visible;
		inline static std::string  s_cursor_icon;
		inline static sf::Vector2u s_cursor_icon_size;
		inline static sf::Vector2u s_cursor_hotspot;

		///
	  /// Resolution independent view width.
	  ///
	[[nodiscard]]
	static auto view_width() noexcept -> int;

///
/// Resolution independent view height.
///
[[nodiscard]]
static auto view_height() noexcept -> int;

///
/// Enable if a keypress will dispatch a single or repeating events.
///
[[nodiscard]]
static auto key_repeat() noexcept -> bool;

///
/// Enable MSAA.
///
[[nodiscard]]
static auto msaa() noexcept -> bool;

///
/// Show/hide mouse cursor.
///
[[nodiscard]]
static auto cursor_visible() noexcept -> bool;

///
/// Cursor texture file in vfs.
///
[[nodiscard]]
static auto cursor_icon() noexcept -> const std::string&;

///
/// Cursor icon texture size.
///
[[nodiscard]]
static auto cursor_icon_size() noexcept -> const sf::Vector2u&;

///
/// Cursor selector point (hotspot).
///
[[nodiscard]]
static auto cursor_hotspot() noexcept -> const sf::Vector2u&;
// config.restore<int>("flag_bitset_count", 8, "misc");

// config.restore<float>("world_to_box2d", 0.01f, "physics");
// config.restore<float>("box2d_to_world", 100.0f, "physics");
// config.restore<int>("flag_bitset_count", 8, "misc");

// config.restore<float>("world_to_box2d", 0.01f, "physics");
// config.restore<float>("box2d_to_world", 100.0f, "physics");

// config.restore<int>("ansiotrophic_filtering", 16, "window");
// config.restore<bool>("highdpi", false, "window");
// s_flag_bitset_count = config.get<int>("flag_bitset_count", "misc");

// s_world_to_box2d = config.get<float>("world_to_box2d", "physics");
// s_box2d_to_world = config.get<float>("box2d_to_world", "physics");
// s_ansio_filtering = config.get<int>("ansiotrophic_filtering", "window");
// s_highdpi         = config.get<bool>("highdpi", "window");
///
/// Number of bits in a flag bitset.
///
//[[nodiscard]]
// static auto flag_bitset_count() noexcept -> int;

///
/// Ratio to convert world coords to box2d.
///
//[[nodiscard]]
// static auto world_to_box2d() noexcept -> float;

///
/// Ratio to convert box2d to world coords.
///
//[[nodiscard]]
// static auto box2d_to_world() noexcept -> float;

///
/// Ansiotropic filtering level.
///
//[[nodiscard]]
// static auto ansio_level() noexcept -> int;

///
/// Enable High DPI support.
///
//[[nodiscard]]
// static auto highdpi() noexcept -> bool;

// inline static int s_flag_bitset_count;

// inline static float  s_world_to_box2d;
// inline static float  s_box2d_to_world;
// inline static int         s_ansio_filtering;
// inline static bool        s_highdpi;

config.restore<float>("ui_font_size", 14.0f);
config.restore<std::string>("default_lang", "en_au");

config.restore<float>("sfx_volume", 1.0f, "audio");
config.restore<float>("music_volume", 1.0f, "audio");
config.restore<float>("dialogue_volume", 1.0f, "audio");
config.restore<int>("listener_count", 1, "audio");

config.restore<bool>("trilinear_filtering", false, "graphics");
config.restore<int>("ansiotrophic_filtering", 2, "graphics");
config.restore<bool>("smaa", false, "graphics.effects");
config.restore<bool>("sharpen", false, "graphics.effects");
config.restore<bool>("gamma_correction", false, "graphics.effects");
config.restore<bool>("chromatic_abberation", false, "graphics.effects");
config.restore<bool>("gaussian_blur", false, "graphics.effects");
config.restore<bool>("film_grain", false, "graphics.effects");

config.restore<int>("camera_foward", input::key_to_int(input::Keys::KEY_W), "input");
config.restore<int>("camera_backward", input::key_to_int(input::Keys::KEY_S), "input");
config.restore<int>("camera_left", input::key_to_int(input::Keys::KEY_A), "input");
config.restore<int>("camera_right", input::key_to_int(input::Keys::KEY_D), "input");
config.restore<int>("camera_rotate_left", input::key_to_int(input::Keys::KEY_Q), "input");
config.restore<int>("camera_rotate_right", input::key_to_int(input::Keys::KEY_E), "input");

///
/// Size of an entity handle.
///
// #define GALAXY_ENTITY_SIZE std::uint64_t
*/

#endif
