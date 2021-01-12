///
/// Log.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include <filesystem>
#include <iostream>

#include "galaxy/platform/Platform.hpp"

#include "Log.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	namespace error
	{
		Log::Log()
		    : m_min_level {error::Level::INFO}, m_message {""}, m_running {false}, m_testing_mode {false}
		{
			platform::configure_terminal();
			std::ios::sync_with_stdio(false);
		}

		Log& Log::get()
		{
			// Singleton instance.
			static Log s_inst;
			return s_inst;
		}

		void Log::start(std::string_view log_file)
		{
			m_running = true;

			// Find path
			auto path = std::filesystem::path {log_file};

			m_file_stream.open(path.string(), std::ofstream::out);
			if (!m_file_stream.good())
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to open file for logging: {0}.", path.string());
			}

			m_thread = std::jthread([&]() {
				// Open stream.
				while (m_running.load())
				{
					m_mutex.lock();

					if (!m_message.empty())
					{
						// Print to stream and std output.
						std::cout << m_message;
						m_file_stream << m_message;

						m_message = "";
					}

					m_mutex.unlock();
				}
			});
		}

		void Log::finish()
		{
			m_running = false;
			m_thread.request_stop();
			m_thread.join();

			m_file_stream.close();
		}

		void Log::set_testing(const bool is_testing)
		{
			m_testing_mode = is_testing;
		}
	} // namespace error
} // namespace galaxy