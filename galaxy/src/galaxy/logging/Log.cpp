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
		Log::Log()
			: m_min_level {LogLevel::INFO}
		{
			m_sinks.reserve(2);
		}

		Log::~Log()
		{
		}

		Log& Log::ref()
		{
			static Log s_instance;
			return s_instance;
		}

		void Log::finish()
		{
			m_min_level = LogLevel::INFO;
			m_sinks.clear();
		}
	} // namespace logging
} // namespace galaxy
