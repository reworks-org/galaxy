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
#include <thread>

namespace DoNotUseReferToConfigForMacroNames
{
	inline static constexpr const std::chrono::duration<double, std::ratio<1, 60>> GALAXY_UPS_VAR {1};

	inline static constexpr const auto GALAXY_DT_VAR =
		static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(GALAXY_UPS_VAR).count()) / 1000.0f;

	inline static auto GALAXY_RESTART_FLAG = false;

	inline static std::chrono::high_resolution_clock::time_point GALAXY_APP_START_TIME_POINT {};
} // namespace DoNotUseReferToConfigForMacroNames

///
/// Defines number of threads allowed for galaxy.
///
#define GALAXY_MAX_THREADS std::thread::hardware_concurrency()

///
/// Default number of threads to use.
///
#define GALAXY_DEFAULT_THREAD_COUNT 4

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
#define GALAXY_RESTART DoNotUseReferToConfigForMacroNames::GALAXY_RESTART_FLAG

///
/// Time point for application start.
///
#define GALAXY_START_TP DoNotUseReferToConfigForMacroNames::GALAXY_APP_START_TIME_POINT

///
/// Updates per second.
///
#define GALAXY_UPS DoNotUseReferToConfigForMacroNames::GALAXY_UPS_VAR

///
/// Galaxy Delta-Time.
///
#define GALAXY_DT DoNotUseReferToConfigForMacroNames::GALAXY_DT_VAR

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
/// Amount of items in a std::vector to initially reserve for the renderer
/// This is to help prevent needless reallocations.
///
#define GALAXY_DEFAULT_RENDERER_RESERVED 1000

///
/// Current root directory of application, unless it has been changed.
///
#define GALAXY_ROOT_DIR std::filesystem::current_path().string()

///
/// Macro for windows platform detection.
///
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(_WIN64)
#define GALAXY_WIN_PLATFORM
#endif

///
/// Macro for linux platform detection.
///
#if defined(__linux__) || defined(__unix) || defined(_POISX_VERSION)
#define GALAXY_LINUX_PLATFORM
#endif

///
/// Macro for macos platform detection.
///
#if defined(__APPLE__) || defined(TARGET_OS_MAC)
#define GALAXY_MACOS_PLATFORM
#endif

#endif