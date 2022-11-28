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

namespace galaxy
{
	struct doNotUseReferToConfigForMacroNames
	{
		static constexpr const std::chrono::duration<double, std::ratio<1, 60>> GALAXY_UPS_VAR {1};

		static constexpr const float GALAXY_DT_VAR =
			static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(GALAXY_UPS_VAR).count()) / 1000.0f;

		static bool GALAXY_RESTART_FLAG;
		static std::chrono::high_resolution_clock::time_point GALAXY_APP_START_TIME_POINT;
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
#define GALAXY_RESTART galaxy::doNotUseReferToConfigForMacroNames::GALAXY_RESTART_FLAG

///
/// Time point for application start.
///
#define GALAXY_START_TP galaxy::doNotUseReferToConfigForMacroNames::GALAXY_APP_START_TIME_POINT

///
/// Updates per second.
///
#define GALAXY_UPS galaxy::doNotUseReferToConfigForMacroNames::GALAXY_UPS_VAR

///
/// Galaxy Delta-Time.
///
#define GALAXY_DT galaxy::doNotUseReferToConfigForMacroNames::GALAXY_DT_VAR

///
/// Number of bits in a flag bitset.
///
#define GALAXY_FLAG_BITSET_COUNT 8

///
/// Extension to use when loading vertex shaders.
///
#define GALAXY_VERTEX_EXT ".vs"

///
/// Extension to use when loading fragment shaders.
///
#define GALAXY_FRAGMENT_EXT ".fs"

///
/// Value used for identity matrix initialization.
///
#define GALAXY_IDENTITY_MATRIX 1.0f

///
/// Index of camera uniform buffer in shaders.
///
#define GAlAXY_CAMERA_UBO_INDEX 0

///
/// Minimum zoom allowed on orthographic camera.
///
#define GALAXY_MIN_CAMERA_ZOOM 0.1f

///
/// Maximum zoom allowed on orthographic camera.
///
#define GALAXY_MAX_CAMERA_ZOOM 1.4f

///
/// Amount of items in a std::vector to initially reserve for the renderer
/// This is to help prevent needless reallocations.
///
#define GALAXY_DEFAULT_RENDERER_RESERVED 1000

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
/// Remove log files older than this amount in hours.
///
#define GALAXY_REMOVE_LOG_FILES_OLDER_THAN_HOURS 48

///
/// Current root directory of application, unless it has been changed.
///
#define GALAXY_APP_DIR std::filesystem::current_path().string()

///
/// Number of threads for the threadpool to use.
///
/// On a 4 core system, you have 1 for main thread, 1 for audio and 2 for tasks.
///
#define GALAXY_WORKER_THREADS 2

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