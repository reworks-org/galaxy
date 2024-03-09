///
/// ConsoleSink.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include <format>
#include <iostream>

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
			auto level_spaced = static_cast<std::string>(message.level);
			level_spaced.push_back(':');
			while (level_spaced.length() < 9)
			{
				level_spaced.push_back(' ');
			}

			std::cout << std::format("{0}[{1}] {2}[{3}] {4}\n",
				message.colour,
				message.time,
				level_spaced,
				std::format("{0}, Ln {1}", message.file, message.line),
				message.message);
		}
	} // namespace error
} // namespace galaxy
