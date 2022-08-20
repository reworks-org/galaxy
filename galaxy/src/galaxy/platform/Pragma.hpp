///
/// Pragma.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PLATFORM_PRAGMA_HPP_
#define GALAXY_PLATFORM_PRAGMA_HPP_

#include "galaxy/utils/Globals.hpp"

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