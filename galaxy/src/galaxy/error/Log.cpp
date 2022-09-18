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
		Log::Log() noexcept
			: m_min_level {LogLevel::INFO}
		{
		}

		Log& Log::handle() noexcept
		{
			static Log s_instance;
			return s_instance;
		}

		void Log::start() noexcept
		{
			m_sinks.reserve(2);
		}

		void Log::finish() noexcept
		{
			m_min_level = LogLevel::INFO;
			m_sinks.clear();
		}
	} // namespace error
} // namespace galaxy