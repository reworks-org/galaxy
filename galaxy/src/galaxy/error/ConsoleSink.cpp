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
	namespace error
	{
		ConsoleSink::ConsoleSink()
		{
		}

		ConsoleSink::~ConsoleSink()
		{
		}

		void ConsoleSink::sink_message(const LogMessage& message)
		{
			std::println("{0}[{1}] [{3}, Ln {4}] {2}: {5}", message.colour, message.time, message.level, message.file, message.line, message.message);
		}
	} // namespace error
} // namespace galaxy
