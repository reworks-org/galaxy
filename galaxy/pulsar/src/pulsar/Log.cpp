///
/// Log.cpp
///
/// pulsar
/// See LICENSE.txt.
///

#include <filesystem>
#include <iostream>

#include "Log.hpp"

using namespace std::chrono_literals;

///
/// Core namespace.
///
namespace pl
{
	Log::Log()
	    : m_min_level {PL_INFO}, m_message {""}, m_running {false}, m_testing_mode {false}
	{
	}

	Log& Log::get() noexcept
	{
		// Singleton instance.
		static Log s_inst;
		return s_inst;
	}

	void Log::start(std::string_view log_file)
	{
		std::ios::sync_with_stdio(false);

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

	void Log::set_testing(const bool is_testing) noexcept
	{
		m_testing_mode = is_testing;
	}

	void Log::set_min_level(Log::Level level) noexcept
	{
		m_min_level = level;
	}

	Log::Level Log::get_min_level() noexcept
	{
		return m_min_level;
	}

	std::string Log::process_level(const Log::Level level)
	{
		std::string out;

		switch (level)
		{
			case PL_INFO:
				out = "INFO";
				break;

			case PL_DEBUG:
				out = "DEBUG";
				break;

			case PL_WARNING:
				out = "WARNING";
				break;

			case PL_ERROR:
				out = "ERROR";
				break;

			case PL_FATAL:
				out = "FATAL";
				break;

			default:
				out = "INVALID MESSAGE LEVEL";
				break;
		}

		return out;
	}

	std::string Log::process_colour(Log::Level level) noexcept
	{
		std::string out;

		switch (level)
		{
			case PL_INFO:
				out = colour_text(LogColours::WHITE);
				break;

			case PL_DEBUG:
				out = colour_text(LogColours::GREEN);
				break;

			case PL_WARNING:
				out = colour_text(LogColours::YELLOW);
				break;

			case PL_ERROR:
				out = colour_text(LogColours::RED);
				break;

			case PL_FATAL:
				out = colour_text(LogColours::FATAL);
				break;

			default:
				out = colour_text(LogColours::WHITE);
				break;
		}

		return out;
	}

	bool Log::filter_level(Log::Level level) noexcept
	{
		bool res = false;

		// Checks for proper stream level.
		if (static_cast<int>(level) >= static_cast<int>(m_min_level))
		{
			res = true;
		}

		return res;
	}
} // namespace pl
