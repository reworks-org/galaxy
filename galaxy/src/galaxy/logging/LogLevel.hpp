///
/// LogLevel.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_LOGGING_LOGLEVEL_HPP_
#define GALAXY_LOGGING_LOGLEVEL_HPP_

#include <format>
#include <string>

#include <magic_enum/magic_enum.hpp>

namespace galaxy
{
	///
	/// Used to determine filtering and colouring of log messages.
	///
	enum class LogLevel : int
	{
		///
		/// Info Log Level.
		///
		INFO = 0,

		///
		/// Warning Log Level.
		///
		WARNING = 1,

		///
		/// Error Log Level.
		///
		ERROR = 2,

		///
		/// Fatal Log Level.
		///
		FATAL = 3
	};

	template<LogLevel level>
	constexpr const char* const get_loglevel_colour()
	{
		if constexpr (level == LogLevel::INFO)
		{
			return "\x1b[1m\x1b[37m";
		}
		else if constexpr (level == LogLevel::WARNING)
		{
			return "\x1b[1m\x1b[33m";
		}
		else if constexpr (level == LogLevel::ERROR)
		{
			return "\x1b[1m\x1b[31m";
		}
		else if constexpr (level == LogLevel::FATAL)
		{
			return "\x1b[41m\x1b[37m";
		}
	}
} // namespace galaxy

template<>
struct std::formatter<galaxy::LogLevel> : std::formatter<std::string>
{
	auto format(const galaxy::LogLevel& level, format_context& ctx) const
	{
		return std::formatter<std::string>::format(std::string {magic_enum::enum_name<galaxy::LogLevel>(level)}, ctx);
	}
};

#endif
