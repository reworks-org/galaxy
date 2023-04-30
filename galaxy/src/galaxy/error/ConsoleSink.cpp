///
/// ConsoleSink.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include <iostream>
#include <format>

#include "ConsoleSink.hpp"

namespace galaxy
{
	namespace error
	{
		void ConsoleSink::sink_message(std::string_view colour,
			std::string_view level,
			std::string_view time,
			std::string_view file,
			std::string_view line,
			std::string_view message)
		{
			std::cout << std::format("{0}[{1}] {2}: [{3}] {4}\n", colour, time, level, std::format("{0}, Ln {1}", file, line), message);
		}
	} // namespace error
} // namespace galaxy