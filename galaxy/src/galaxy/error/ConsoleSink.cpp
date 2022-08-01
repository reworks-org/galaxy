///
/// ConsoleSink.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include <iostream>

#include "ConsoleSink.hpp"

namespace galaxy
{
	namespace error
	{
		void ConsoleSink::sink_message(std::string_view message) noexcept
		{
			std::cout << message;
		}
	} // namespace error
} // namespace galaxy