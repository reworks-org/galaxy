///
/// ConsoleSink.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include <print>

#include "ConsoleSink.hpp"

namespace galaxy
{
	namespace logging
	{
		ConsoleSink::ConsoleSink() noexcept
		{
		}

		ConsoleSink::~ConsoleSink() noexcept
		{
		}

		void ConsoleSink::sink_message(const LogMessage& message)
		{
			std::println("{0}[{1}] [{3}, Ln {4}] {2}: {5}\x1b[0m", message.colour, message.time, message.level, message.file, message.line, message.message);
		}
	} // namespace logging
} // namespace galaxy
