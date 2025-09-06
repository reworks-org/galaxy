///
/// Log.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include <SDL3/SDL_log.h>

#include "Log.hpp"

namespace galaxy
{
	void sdl_log_callback(void* userdata, int category, SDL_LogPriority priority, const char* message)
	{
		switch (priority)
		{
			case SDL_LOG_PRIORITY_INVALID:
			case SDL_LOG_PRIORITY_TRACE:
			case SDL_LOG_PRIORITY_VERBOSE:
			case SDL_LOG_PRIORITY_DEBUG:
			case SDL_LOG_PRIORITY_INFO:
				GALAXY_LOG(GALAXY_INFO, message);
				break;
			case SDL_LOG_PRIORITY_WARN:
				GALAXY_LOG(GALAXY_WARN, message);
				break;
			case SDL_LOG_PRIORITY_ERROR:
			case SDL_LOG_PRIORITY_CRITICAL:
				GALAXY_LOG(GALAXY_ERROR, message);
				break;
			default:
				GALAXY_LOG(GALAXY_INFO, message);
				break;
		}
	}

	Log::Log() noexcept
		: m_min_level {LogLevel::INFO}
	{
		m_sinks.reserve(2);
		SDL_SetLogOutputFunction(&sdl_log_callback, nullptr);
	}

	Log::~Log() noexcept
	{
		// We reset min level because logging is done as a singleton.
		m_min_level = LogLevel::INFO;
		m_sinks.clear();
	}
} // namespace galaxy
