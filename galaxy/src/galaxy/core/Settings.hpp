///
/// Settings.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_SETTINGS_HPP_
#define GALAXY_CORE_SETTINGS_HPP_

#include "galaxy/core/Config.hpp"
#include "galaxy/graphics/ScreenMode.hpp"

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
		/// \param dt Config option to restore defaults to.
		///
		static auto set_config_to_default(core::Config& config) -> void;

		///
		/// Set all our settings using the provided config file.
		///
		/// \param dt Config option to set settings from.
		///
		static auto set_settings_from_config(core::Config& config) -> void;

		///
		/// Set galaxy delta time.
		///
		/// \param dt Time lag from game loop.
		///
		static auto set_delta_time(const double dt) -> void;

		///
		/// Get galaxy delta time.
		///
		/// \return Current lag from gameloop.
		///
		[[nodiscard]]
		static auto dt() -> double;

		///
		/// Number of bits in a flag bitset.
		///
		[[nodiscard]]
		static auto flag_bitset_count() -> int;

		///
		/// Successful return.
		///
		[[nodiscard]]
		static auto exit_success() -> int;

		///
		/// Failed main return.
		///
		[[nodiscard]]
		static auto exit_failure() -> int;

		///
		/// Ratio to convert world coords to box2d.
		///
		[[nodiscard]]
		static auto world_to_box2d() -> float;

		///
		/// Ratio to convert box2d to world coords.
		///
		[[nodiscard]]
		static auto box2d_to_world() -> float;

		///
		/// Window creation width.
		///
		[[nodiscard]]
		static auto window_width() -> int;

		///
		/// Window creation height.
		///
		[[nodiscard]]
		static auto window_height() -> int;

		///
		/// Window title.
		///
		[[nodiscard]]
		static auto window_title() -> const std::string&;

		///
		/// Window fullscreen mode.
		///
		[[nodiscard]]
		static auto screenmode() -> graphics::ScreenMode;

		///
		/// Vsync control.
		///
		[[nodiscard]]
		static auto vsync() -> bool;

		///
		/// Show/hide mouse cursor.
		///
		[[nodiscard]]
		static auto mouse_visible() -> bool;

		///
		/// Enable default 4x MSAA.
		///
		[[nodiscard]]
		static auto msaa() -> bool;

		///
		/// Enable High DPI support.
		///
		[[nodiscard]]
		static auto highdpi() -> bool;

		///
		/// Window icon file in vfs.
		///
		[[nodiscard]]
		static auto window_icon() -> const std::string&;

		///
		/// Cursor texture file in vfs.
		///
		[[nodiscard]]
		static auto cursor_icon() -> const std::string&;

		///
		/// Game updates per second, independant of FPS, see "fixed timestep gameloop".
		///
		[[nodiscard]]
		static auto ups() -> double;

		///
		/// Current root directory of application, unless it has been changed.
		///
		[[nodiscard]]
		static auto root_dir() -> std::filesystem::path;

		///
		/// Main data directory.
		///
		[[nodiscard]]
		static auto assets_dir() -> std::filesystem::path;

		///
		/// Directory for editor specific stuff.
		///
		[[nodiscard]]
		static auto editor_dir() -> std::filesystem::path;

		///
		/// Name of packed assets file.
		///
		[[nodiscard]]
		static auto asset_pack() -> const std::string&;

		///
		/// Should asset data be read from pack or assets dir.
		///
		[[nodiscard]]
		static auto use_loose_assets() -> bool;

		///
		/// Music asset location.
		///
		[[nodiscard]]
		static auto assets_dir_music() -> const std::string&;

		///
		/// SFX asset location.
		///
		[[nodiscard]]
		static auto assets_dir_sfx() -> const std::string&;

		///
		/// Voice asset location.
		///
		[[nodiscard]]
		static auto assets_dir_voice() -> const std::string&;

		///
		/// Font asset location.
		///
		[[nodiscard]]
		static auto assets_dir_font() -> const std::string&;

		///
		/// Scripts asset location.
		///
		[[nodiscard]]
		static auto assets_dir_script() -> const std::string&;

		///
		/// Shaders asset location.
		///
		[[nodiscard]]
		static auto assets_dir_shaders() -> const std::string&;

		///
		/// Animation data location.
		///
		[[nodiscard]]
		static auto assets_dir_animation() -> const std::string&;

		///
		/// Textures asset location.
		///
		[[nodiscard]]
		static auto assets_dir_texture() -> const std::string&;

		///
		/// Prefab asset location.
		///
		[[nodiscard]]
		static auto assets_dir_prefabs() -> const std::string&;

		///
		/// Maps asset location.
		///
		[[nodiscard]]
		static auto assets_dir_maps() -> const std::string&;

		///
		/// Video asset location.
		///
		[[nodiscard]]
		static auto assets_dir_video() -> const std::string&;

		///
		/// UI asset location.
		///
		[[nodiscard]]
		static auto assets_dir_ui() -> const std::string&;

	private:
		inline static double s_delta_time;

		inline static int s_flag_bitset_count;

		inline static float  s_world_to_box2d;
		inline static float  s_box2d_to_world;
		inline static double s_ups;

		inline static int                  s_window_width;
		inline static int                  s_window_height;
		inline static std::string          s_title;
		inline static graphics::ScreenMode s_screenmode;
		inline static bool                 s_vsync;
		inline static bool                 s_mouse_visible;
		inline static bool                 s_msaa;
		inline static bool                 s_highdpi;
		inline static std::string          s_icon;
		inline static std::string          s_cursor_icon;

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

///
/// Size of an entity handle.
///
#define GALAXY_ENTITY_SIZE std::uint64_t

///
/// ZLib inflate/deflate chunk size.
///
#define GALAXY_ZLIB_COMPLETE_CHUNK 16384

///
/// Prevents compiler warnings when applied to unused parameters.
///
#define GALAXY_UNUSED(var) ((void)(var))

///
/// Macro for windows platform detection.
///
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(_WIN64) || defined(_MSC_VER)
#define GALAXY_WIN_PLATFORM
#endif

///
/// Macro for linux platform detection.
///
#if defined(__linux__) || defined(__unix) || defined(_POISX_VERSION) || defined(__GNUC__) || defined(__clang__)
#define GALAXY_LINUX_PLATFORM
#endif

///
/// Macro for macos platform detection.
///
#if defined(__APPLE__) || defined(TARGET_OS_MAC)
#define GALAXY_MACOS_PLATFORM
#endif

#endif

/*

config.restore<float>("ui_font_size", 14.0f);
config.restore<bool>("maximized", false, "window");
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
/// Galaxy appdata file.
///
#define GALAXY_APPDATA 	inline static const std::string app_data {"app.galaxy"};

*/
