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
	namespace error
	{
		Log::Log()
			: m_min_level {LogLevel::INFO}
		{
		}

		Log& Log::handle()
		{
			static Log s_instance;
			return s_instance;
		}

		void Log::start()
		{
			m_sinks.reserve(20);
		}

		void Log::finish()
		{
			m_min_level = LogLevel::INFO;
			m_sinks.clear();
		}
	} // namespace error
} // namespace galaxy