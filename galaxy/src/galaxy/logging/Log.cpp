///
/// Log.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include "Log.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	Log::Log() noexcept
		: m_min_level {LogLevel::INFO}
	{
		m_sinks.reserve(2);
	}

	Log::~Log() noexcept
	{
		// We reset min level because logging is done as a singleton.
		m_min_level = LogLevel::INFO;
		m_sinks.clear();
	}
} // namespace galaxy
