///
/// Globals.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_GLOBALS_HPP_
#define GALAXY_META_GLOBALS_HPP_

#include <chrono>
#include <filesystem>
#include <thread>

namespace
{
	inline static constexpr const std::chrono::duration<double, std::ratio<1, 60>> GALAXY_UPS_VAR {1};

	inline static constexpr const auto GALAXY_DT_VAR =
		static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(GALAXY_UPS_VAR).count()) / 1000.0f;

	inline static auto GALAXY_RESTART_FLAG = false;

	inline static std::chrono::high_resolution_clock::time_point GALAXY_APP_START_TIME_POINT {};
} // namespace

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
#define GALAXY_RESTART GALAXY_RESTART_FLAG

///
/// Time point for application start.
///
#define GALAXY_START_TP GALAXY_APP_START_TIME_POINT

///
/// Updates per second.
///
#define GALAXY_UPS GALAXY_UPS_VAR

///
/// Galaxy Delta-Time.
///
#define GALAXY_DT GALAXY_DT_VAR

///
/// Current root directory of application, unless it has been manipulated.
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