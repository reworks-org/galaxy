///
/// RaylibError.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <raylib.h>

#include "galaxy/logging/Log.hpp"

#include "RaylibError.hpp"

namespace galaxy
{
	namespace logging
	{
		void raylib_trace(int level, const char* text, va_list args)
		{
			std::string result;

			va_start(args, text);
			char   buf[32];
			size_t n = std::vsnprintf(buf, sizeof(buf), text, args);
			va_end(args);

			if (n < sizeof(buf))
			{
				// Buffer is already fine size.
				result = {buf, n};
			}
			else
			{
				// Need larger buffer.
				result = std::string(n + 1, 0);
				va_start(args, text);
				std::vsnprintf(const_cast<char*>(result.data()), result.size(), text, args);
				va_end(args);
			}

			switch (level)
			{
				case LOG_INFO:
					GALAXY_LOG(GALAXY_INFO, result);
					break;
				case LOG_ERROR:
					GALAXY_LOG(GALAXY_ERROR, result);
					break;
				case LOG_WARNING:
					GALAXY_LOG(GALAXY_WARNING, result);
					break;
				case LOG_DEBUG:
					GALAXY_LOG(GALAXY_DEBUG, result);
					break;
				default:
					break;
			}
		}
	} // namespace logging
} // namespace galaxy
