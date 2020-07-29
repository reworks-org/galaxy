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

	Log::~Log() noexcept
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
		m_running = true;

		// Find path
		auto path = std::filesystem::path {static_cast<std::string>(log_file)};
		auto dir  = path.parent_path();

		// Make sure it exists.
		if (!std::filesystem::exists(dir))
		{
			std::filesystem::create_directory(dir);
		}

		m_thread = std::async(std::launch::async, [&]() {
			// Open stream.
			m_file_stream.open(path.string(), std::ofstream::out);

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

		std::lock_guard<std::mutex> lock {m_mutex};
		m_file_stream.close();

		m_thread.get();
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
		std::string out {""};

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

	std::string Log::process_colour(Log::Level level)
	{
		std::string out {""};

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
		// Checks for proper stream level.
		if (static_cast<int>(level) >= static_cast<int>(m_min_level))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
} // namespace pl