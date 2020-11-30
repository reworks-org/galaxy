///
/// Log.cpp
///
/// pulsar
/// See LICENSE.txt.
///

#include <filesystem>
#include <iostream>

#ifdef _WIN64
#include <windows.h>
#endif

#include "Log.hpp"

using namespace std::chrono_literals;

///
/// Core namespace.
///
namespace pl
{
	Log::Log()
	    : m_min_level {log_level::Level::INFO}, m_message {""}, m_running {false}, m_testing_mode {false}
	{
		// clang-format off
        #ifdef _WIN64
		    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD mode = 0;

			GetConsoleMode(handle, &mode);
			mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			SetConsoleMode(handle, mode);
        #endif
		// clang-format on

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
			PL_LOG(PL_FATAL, "Failed to open file for logging: {0}.", path.string());
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
} // namespace pl
