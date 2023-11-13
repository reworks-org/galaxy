///
/// Globals.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UTILS_GLOBALS_HPP_
#define GALAXY_UTILS_GLOBALS_HPP_

#include <chrono>
#include <filesystem>

#include "galaxy/meta/Memory.hpp"

namespace galaxy
{
	struct DoNotUseReferToMacros
	{
		inline static constexpr const std::chrono::duration<double, std::ratio<1, 60>> GALAXY_UPS_VAR {1};

		inline static constexpr const float GALAXY_DT_VAR =
			static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(GALAXY_UPS_VAR).count()) / 1000.0f;

		inline static bool GALAXY_RESTART_FLAG {false};

		inline static unsigned int GALAXY_CUR_UPS_VAR {0};
		inline static unsigned int GALAXY_CUR_FPS_VAR {0};

		inline static const std::string data {"data/"};
		inline static const std::string data_mods {"data/mods/"};
		inline static const std::string data_updates {"data/updates/"};
		inline static const std::string work_dir {"assets/"};
		inline static const std::string editor_data_dir {"editor/"};
		inline static const std::string assets_music {"music/"};
		inline static const std::string assets_sfx {"sfx/"};
		inline static const std::string assets_voice {"voice/"};
		inline static const std::string assets_font {"fonts/"};
		inline static const std::string assets_script {"scripts/"};
		inline static const std::string assets_shader {"shaders/"};
		inline static const std::string assets_texture {"textures/"};
		inline static const std::string assets_atlas {"atlas/"};
		inline static const std::string assets_lang {"lang/"};
		inline static const std::string assets_prefabs {"prefabs/"};
		inline static const std::string assets_maps {"maps/"};
		inline static const std::string assets_video {"video/"};
		inline static const std::string assets_ui {"ui/"};
		inline static const std::string assets_ui_fonts {"ui/fonts/"};
	};
} // namespace galaxy

///
/// Prevents compiler warnings when applied to unused parameters.
///
#define GALAXY_UNUSED(var) ((void)(var))

///
/// Size of an entity handle.
///
#define GALAXY_ENTITY_SIZE std::uint64_t

///
/// Restart flag.
///
#define GALAXY_RESTART galaxy::DoNotUseReferToMacros::GALAXY_RESTART_FLAG

///
/// Updates per second.
///
#define GALAXY_UPS galaxy::DoNotUseReferToMacros::GALAXY_UPS_VAR

///
/// Galaxy Delta-Time.
///
#define GALAXY_DT galaxy::DoNotUseReferToMacros::GALAXY_DT_VAR

///
/// GALAXY current UPS.
///
#define GALAXY_CUR_UPS galaxy::DoNotUseReferToMacros::GALAXY_CUR_UPS_VAR

///
/// Galaxy current FPS.
///
#define GALAXY_CUR_FPS galaxy::DoNotUseReferToMacros::GALAXY_CUR_FPS_VAR

///
/// Number of bits in a flag bitset.
///
#define GALAXY_FLAG_BITSET_COUNT 8

///
/// Value used for identity matrix initialization.
///
#define GALAXY_IDENTITY_MATRIX 1.0f

///
/// Index of camera uniform buffer in shaders.
///
#define GAlAXY_UBO_CAMERA_INDEX 0

///
/// Index of render 2d uniform buffer in shaders.
///
#define GAlAXY_UBO_R2D_INDEX 1

///
/// Index for SSBO light data.
///
#define GALAXY_SSBO_LIGHTING_INDEX 2

///
/// Minimum zoom allowed on orthographic camera.
///
#define GALAXY_MIN_CAMERA_ZOOM 0.1f

///
/// Maximum zoom allowed on orthographic camera.
///
#define GALAXY_MAX_CAMERA_ZOOM 1.4f

///
/// Specifies the width of the range around the shape between the minimum and maximum
/// representable signed distance in shape units or distance field pixels.
///
#define GALAXY_FONT_MSDF_RANGE 4.0f

///
/// Sets the scale used to convert shape units to distance field pixels.
///
#define GALAXY_FONT_MSDF_SCALE 2.0f

///
/// Default number of fragments to use when drawing an ellipse.
///
#define GALAXY_DEFAULT_ELLIPSE_FRAGMENTS 50

///
/// Ratio to convert world coords to box2d.
///
#define GALAXY_WORLD_TO_BOX 0.01f

///
/// Ratio to convert box2d to world coords.
///
#define GALAXY_BOX_TO_WORLD 100.0f

///
/// Current root directory of application, unless it has been changed.
///
#define GALAXY_ROOT_DIR std::filesystem::current_path()

///
/// Main data directory.
///
#define GALAXY_DATA_DIR galaxy::DoNotUseReferToMacros::data

///
/// Directory for working assets.
///
#define GALAXY_WORK_DIR galaxy::DoNotUseReferToMacros::work_dir

///
/// Directory for mods.
///
#define GALAXY_MOD_DIR galaxy::DoNotUseReferToMacros::data_mods

///
/// Directory for update archives.
///
#define GALAXY_UPDATE_DIR galaxy::DoNotUseReferToMacros::data_updates

///
/// Directory for galaxy editor data.
///
#define GALAXY_EDITOR_DATA_DIR galaxy::DoNotUseReferToMacros::editor_data_dir

///
/// Music asset location.
///
#define GALAXY_MUSIC_DIR galaxy::DoNotUseReferToMacros::assets_music

///
/// SFX asset location.
///
#define GALAXY_SFX_DIR galaxy::DoNotUseReferToMacros::assets_sfx

///
/// Voice asset location.
///
#define GALAXY_VOICE_DIR galaxy::DoNotUseReferToMacros::assets_voice

///
/// Font asset location.
///
#define GALAXY_FONT_DIR galaxy::DoNotUseReferToMacros::assets_font

///
/// Script asset location.
///
#define GALAXY_SCRIPT_DIR galaxy::DoNotUseReferToMacros::assets_script

///
/// Shader asset location.
///
#define GALAXY_SHADER_DIR galaxy::DoNotUseReferToMacros::assets_shader

///
/// Texture asset location.
///
#define GALAXY_TEXTURE_DIR galaxy::DoNotUseReferToMacros::assets_texture

///
/// Atlas asset location.
///
#define GALAXY_ATLAS_DIR galaxy::DoNotUseReferToMacros::assets_atlas

///
/// Lang asset location.
///
#define GALAXY_LANG_DIR galaxy::DoNotUseReferToMacros::assets_lang

///
/// Prefab asset location.
///
#define GALAXY_PREFABS_DIR galaxy::DoNotUseReferToMacros::assets_prefabs

///
/// Map asset location.
///
#define GALAXY_MAPS_DIR galaxy::DoNotUseReferToMacros::assets_maps

///
/// Video asset location.
///
#define GALAXY_VIDEO_DIR galaxy::DoNotUseReferToMacros::assets_video

///
/// UI asset location.
///
#define GALAXY_UI_DIR galaxy::DoNotUseReferToMacros::assets_ui

///
/// UI fonts asset location.
///
#define GALAXY_UI_FONTS_DIR galaxy::DoNotUseReferToMacros::assets_ui_fonts

///
/// ZLib inflate/deflate chunk size.
///
#define GALAXY_ZLIB_COMPLETE_CHUNK 16384

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

///
/// Successful return.
///
#define GALAXY_EXIT_SUCCESS 0

///
/// Failed main return.
///
#define GALAXY_EXIT_FAILURE 1

#endif
