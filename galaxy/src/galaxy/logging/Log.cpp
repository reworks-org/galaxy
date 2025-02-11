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
	namespace logging
	{
		Log::Log() noexcept
			: m_min_level {LogLevel::_INFO_}
		{
			m_sinks.reserve(2);
		}

		Log::~Log() noexcept
		{
			m_min_level = LogLevel::_INFO_;
			m_sinks.clear();
		}
	} // namespace logging
} // namespace galaxy
