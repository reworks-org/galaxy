///
/// Log.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include <cstdarg>

#include <Raylib.hpp>

#include "Log.hpp"

namespace galaxy
{
	Log::Log() noexcept
		: m_min_level {LogLevel::INFO}
	{
		m_sinks.reserve(2);

		ray::SetTraceLogLevel(ray::LOG_DEBUG);
		ray::SetTraceLogCallback([](int msgType, const char* text, va_list args) {
			va_list args_copy;
			va_copy(args_copy, args);

			const auto size = vsnprintf(nullptr, 0, text, args_copy);
			va_end(args_copy);

			std::string result(size, '\0');
			vsnprintf(result.data(), result.size() + 1, text, args);

			switch (msgType)
			{
				case ray::LOG_DEBUG:
				case ray::LOG_INFO:
					GALAXY_LOG(GALAXY_INFO, "[Raylib] {0}", result);
					break;

				case ray::LOG_WARNING:
					GALAXY_LOG(GALAXY_WARN, "[Raylib] {0}", result);
					break;

				case ray::LOG_ERROR:
					GALAXY_LOG(GALAXY_ERROR, "[Raylib] {0}", result);
					break;

				case ray::LOG_FATAL:
					GALAXY_LOG(GALAXY_FATAL, "[Raylib] {0}", result);
					break;

				default:
					GALAXY_LOG(GALAXY_ERROR, "Unknown raylib log message level.");
					break;
			}
		});
	}

	Log::~Log() noexcept
	{
		// We reset min level because logging is done as a singleton.
		m_min_level = LogLevel::INFO;
		m_sinks.clear();
	}
} // namespace galaxy
