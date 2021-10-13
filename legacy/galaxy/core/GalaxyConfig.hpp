///
/// GalaxyConfig.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>

#ifndef GALAXY_CORE_GALAXYCONFIG_HPP_
#define GALAXY_CORE_GALAXYCONFIG_HPP_

///
/// Prevents compiler warnings when applied to unused parameters.
///
#define GALAXY_UNUSED(var) ((void)(var))

///
/// Size of an entity handle.
///
#define GALAXY_ENTITY_SIZE std::uint64_t

///
/// Updates per second.
///
inline static constexpr const std::chrono::duration<double, std::ratio<1, 60>> GALAXY_UPS_VAR {1};
#define GALAXY_UPS GALAXY_UPS_VAR

///
/// Galaxy Delta-Time.
///
inline static constexpr const auto GALAXY_DT_VAR = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(GALAXY_UPS).count()) / 1000.0f;
#define GALAXY_DT GALAXY_DT_VAR

///
/// Macro for windows platform detection.
///
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(_WIN64)
#define GALAXY_WIN_PLATFORM
#endif

///
/// Macro for unix platform detection.
///
#if defined(__APPLE__) || defined(__linux__) || defined(__unix) || defined(_POISX_VERSION) || defined(TARGET_OS_MAC)
#define GALAXY_UNIX_PLATFORM
#endif

#endif