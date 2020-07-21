///
/// Log.cpp
///
/// pulsar
/// See LICENSE.txt.
///

#include "Log.hpp"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <thread>

#include <date/tz.h>
#include <fmt/format.h>

using namespace std::chrono_literals;

///
/// Core namespace.
///
namespace pl
{
	Log::Log() noexcept
	    : m_minimumLevel(PL_INFO), m_curMessage(""), m_running(false), m_testingMode(false)
	{
	}

	Log::~Log()
	{
		if (m_running)
		{
			throw std::runtime_error("Failed to call Log::finish()!");
		}
	}

	Log& Log::get() noexcept
	{
		// Singleton instance.
		static Log s_inst;
		return s_inst;
	}

	void Log::start(std::string_view _log)
	{
		m_running = true;

		// Find path
		auto path      = std::filesystem::path(static_cast<std::string>(_log));
		auto directory = path.parent_path();

		// Make sure it exists.
		if (!std::filesystem::exists(directory))
		{
			std::filesystem::create_directory(directory);
		}

		m_thread = std::async(std::launch::async, [&]() {
			// Open stream.
			m_fileStream.open(path.string(), std::ofstream::out);

			while (m_running.load())
			{
				m_mutex.lock();

				if (m_curMessage != "")
				{
					// Print to stream and std output.
					std::cout << m_curMessage;
					m_fileStream << m_curMessage;

					m_curMessage = "";
				}

				m_mutex.unlock();
			}
		});
	}

	void Log::finish()
	{
		m_running = false;

		std::lock_guard<std::mutex> l_lock(m_mutex);
		m_fileStream.close();

		m_thread.get();
	}

	void Log::log(const Log::Level level, const std::string& message)
	{
		if (!m_testingMode)
		{
			// Check to make sure level should be logged.
			if (Log::get().filterLevel(level))
			{
				std::lock_guard<std::mutex> l_lock(m_mutex);

				if (m_curMessage == "")
				{
					// Create log message string.
					m_curMessage = fmt::format("{0}[{1}] - {2} - {3}\n", Log::get().processColour(level), Log::get().processLevel(level), date::format("%m/%d/%Y %H:%M\n", date::make_zoned(date::current_zone(), std::chrono::system_clock::now())), message);

					if (level == PL_FATAL)
					{
						throw std::runtime_error(m_curMessage);
					}
				}
			}
		}
	}

	void Log::setTestingMode(const bool isTestingMode) noexcept
	{
		m_testingMode = isTestingMode;
	}

	void Log::setMinimumLevel(Log::Level level) noexcept
	{
		m_minimumLevel = level;
	}

	Log::Level Log::getMinimumLevel() noexcept
	{
		return m_minimumLevel;
	}

	std::string Log::processLevel(const Log::Level level) noexcept
	{
		std::string out = "";

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

		return std::move(out);
	}

	std::string Log::processColour(Log::Level level) noexcept
	{
		std::string out = "";

		switch (level)
		{
			case PL_INFO:
				out = colourText(LogColours::WHITE);
				break;

			case PL_DEBUG:
				out = colourText(LogColours::GREEN);
				break;

			case PL_WARNING:
				out = colourText(LogColours::YELLOW);
				break;

			case PL_ERROR:
				out = colourText(LogColours::RED);
				break;

			case PL_FATAL:
				out = colourText(LogColours::FATAL);
				break;

			default:
				out = colourText(LogColours::WHITE);
				break;
		}

		return std::move(out);
	}

	bool Log::filterLevel(Log::Level level) noexcept
	{
		// Checks for proper stream level.
		if (static_cast<int>(level) >= static_cast<int>(m_minimumLevel))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
} // namespace pl