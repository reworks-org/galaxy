///
/// LogMessage.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_LOGGING_LOGMESSAGE_HPP_
#define GALAXY_LOGGING_LOGMESSAGE_HPP_

#include "galaxy/logging/LogLevel.hpp"

namespace galaxy
{
	///
	/// Parts of a log message to be passed to sinks.
	///
	struct LogMessage final
	{
		///
		/// ASCII colour code.
		///
		std::string colour;

		///
		/// Level of message.
		///
		LogLevel level;

		///
		/// Timestamp of message.
		///
		std::string time;

		///
		/// File message occured in.
		///
		std::string file;

		///
		/// Line the message occured on.
		///
		unsigned int line;

		///
		/// The actual message to record.
		///
		std::string message;

		///
		/// Stack trace.
		///
		std::string trace;
	};

} // namespace galaxy

template<>
struct std::formatter<galaxy::LogMessage> : std::formatter<std::string>
{
	auto format(const galaxy::LogMessage& lm, format_context& ctx) const noexcept
	{
		return std::formatter<std::string>::format(std::format("{0}[{1}] [{3}, Ln {4}] {2}: {5}\x1b[0m", lm.colour, lm.time, lm.level, lm.file, lm.line, lm.message), ctx);
	}
};

#endif
