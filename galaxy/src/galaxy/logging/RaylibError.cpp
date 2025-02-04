///
/// RaylibError.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <raylib.hpp>

#include "galaxy/logging/Log.hpp"
#include "galaxy/platform/Pragma.hpp"

#include "RaylibError.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26826)
GALAXY_DISABLE_WARNING(26477)
GALAXY_DISABLE_WARNING(26465)
#endif

namespace galaxy
{
	namespace logging
	{
		std::string handle_va(const char* text, va_list args, ...)
		{
			std::string result;

			va_start(args, text);
			char         buf[32];
			const size_t n = std::vsnprintf(buf, sizeof(buf), text, args);
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

			return result;
		}

		void raylib_trace(int level, const char* text, va_list args)
		{
			const auto result = handle_va(text, args);
			switch (level)
			{
				case ray::TraceLogLevel::LOG_INFO:
					GALAXY_LOG(GALAXY_INFO, result);
					break;
				case ray::TraceLogLevel::LOG_ERROR:
					GALAXY_LOG(GALAXY_ERROR, result);
					break;
				case ray::TraceLogLevel::LOG_WARNING:
					GALAXY_LOG(GALAXY_WARNING, result);
					break;
				case ray::TraceLogLevel::LOG_DEBUG:
					GALAXY_LOG(GALAXY_DEBUG, result);
					break;
				default:
					break;
			}
		}
	} // namespace logging
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif
