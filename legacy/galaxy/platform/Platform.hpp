///
/// Platform.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_PLATFORM_PLATFORM_HPP_
#define GALAXY_PLATFORM_PLATFORM_HPP_

#include "galaxy/core/GalaxyConfig.hpp"

#ifdef GALAXY_WIN_PLATFORM
#include "galaxy/platform/specific/Windows.hpp"
#elif GALAXY_UNIX_PLATFORM
#include "galaxy/platform/specific/Unix.hpp"
#endif

#endif