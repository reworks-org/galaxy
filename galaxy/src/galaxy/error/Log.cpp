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
		    : m_min_level {0}
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

			// clang-format off
			m_thread = std::jthread([&]() {
				while (m_file_stream.is_open())
				{
					std::this_thread::sleep_for(1s);
					
					std::lock_guard<std::mutex> lock {m_msg_mutex};
					*m_stream << m_message;
					m_file_stream << m_message;
					m_message.clear();
				}
			});
			// clang-format on
		}

		void Log::finish()
		{
			m_file_stream.close();
			m_thread.request_stop();
			m_thread.join();
		}

		void Log::change_stream(std::ostream& stream)
		{
			std::lock_guard<std::mutex> lock {m_msg_mutex};
			m_stream = &stream;
		}
	} // namespace error
} // namespace galaxy