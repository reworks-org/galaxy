///
/// Pragma.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PLATFORM_PRAGMA_HPP_
#define GALAXY_PLATFORM_PRAGMA_HPP_

///
/// Prevents compiler warnings when applied to unused parameters.
///
#define GALAXY_UNUSED(var) ((void)(var))

#if defined(_DEBUG) || defined(DEBUG)
///
/// Define a constexpr compatible debug macro.
///
#define GALAXY_DEBUG_BUILD true
#elif defined(_NDEBUG) || defined(NDEBUG)
///
/// Define a constexpr compatible debug macro.
///
#define GALAXY_DEBUG_BUILD false
#endif

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

#ifdef GALAXY_WIN_PLATFORM
#define GALAXY_PLATFORM_PUSH       __pragma(warning(push))
#define GALAXY_PLATFORM_POP        __pragma(warning(pop))
#define GALAXY_PLATFORM_WARNING(x) __pragma(warning(disable : x))
#elif GALAXY_UNIX_PLATFORM
#define GALAXY_PLATFORM_PUSH       _Pragma("GCC diagnostic push")
#define GALAXY_PLATFORM_POP        _Pragma("GCC diagnostic pop")
#define GALAXY_DO_PRAGMA(x)        _Pragma(#x)
#define GALAXY_PLATFORM_WARNING(x) GALAXY_DO_PRAGMA(GCC diagnostic ignored #x)
#endif

#define GALAXY_DISABLE_WARNING_PUSH GALAXY_PLATFORM_PUSH
#define GALAXY_DISABLE_WARNING_POP  GALAXY_PLATFORM_POP
#define GALAXY_DISABLE_WARNING(x)   GALAXY_PLATFORM_WARNING(x)

#endif
