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
	ConsoleSink::ConsoleSink() noexcept
	{
	}

	ConsoleSink::~ConsoleSink() noexcept
	{
	}

	void ConsoleSink::sink(const LogMessage& message)
	{
		std::println("{}", message);
	}
} // namespace galaxy
