///
/// Log.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include <execution>
#include <filesystem>
#include <iostream>

#include "galaxy/platform/Platform.hpp"

#include "Log.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	namespace error
	{
		Log::Log() noexcept
		    : m_min_level {0}, m_started {false}
		{
			m_stream = &std::cout;
			platform::configure_terminal();
		}

		Log& Log::handle() noexcept
		{
			static Log s_inst;
			return s_inst;
		}

		void Log::start(std::string_view log_file)
		{
			auto path = std::filesystem::path {log_file};
			m_file_stream.open(path.string(), std::ofstream::out);
			m_started = true;
		}

		void Log::finish()
		{
			m_file_stream.close();
			m_started = false;
		}

		void Log::change_stream(std::ostream& ostream)
		{
			m_stream = &ostream;
		}
	} // namespace error
} // namespace galaxy