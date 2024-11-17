///
/// RaylibError.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LOGGING_RAYLIBERROR_HPP_
#define GALAXY_LOGGING_RAYLIBERROR_HPP_

#include <cstdarg>

namespace galaxy
{
	namespace logging
	{
		void raylib_trace(int level, const char* text, va_list args);
	} // namespace logging
} // namespace galaxy

#endif
