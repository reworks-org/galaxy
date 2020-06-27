///
/// Log.cpp
///
/// pulsar
/// See LICENSE.txt.
///

#include <ctime>
#include <chrono>
#include <thread>
#include <iostream>
#include <algorithm>
#include <filesystem>

#include "Log.hpp"

using namespace std::chrono_literals;

///
/// Core namespace.
///
namespace pulsar
{
	Log::Log() noexcept
		:m_minimumLevel(PL_INFO), m_testingMode(false)
	{
	}

	Log::~Log() noexcept
	{
		deinit();
	}

	Log& Log::get() noexcept
	{
		// Singleton instance.
		static Log s_inst;
		return s_inst;
	}

	void Log::init(const std::string& logTo) noexcept
	{
		// allow for logging to complete.
		std::this_thread::sleep_for(50ms);

		// Find path
		std::filesystem::path path(logTo);
		std::filesystem::path directory = path.remove_filename();

		// Make sure it exists.
		if (!std::filesystem::exists(directory))
		{
			std::filesystem::create_directory(directory);
		}

		// Open stream.
		m_fileStream.open(logTo, std::ofstream::out);
	}

	void Log::deinit() noexcept
	{
		m_fileStream.close();
	}

	void Log::log(const pulsar::Log::Level level, const std::string& message) noexcept
	{
		if (!m_testingMode)
		{
			// Check to make sure level should be logged.
			if (pulsar::Log::get().filterLevel(level))
			{
				std::lock_guard<std::mutex> l_lock(m_mutex);

				// Prefix string
				std::string output = pulsar::Log::get().processColour(level) + "[" + pulsar::Log::get().processLevel(level) + "] - " + pulsar::Log::get().getDateTime() + " - ";
				output += (message + "\n");

				// Print to stream and std output.
				std::cout << output;
				m_fileStream << output;
			}
		}
	}

	void Log::setTestingMode(const bool isTestingMode) noexcept
	{
		m_testingMode = isTestingMode;
	}

	void Log::setMinimumLevel(pulsar::Log::Level level) noexcept
	{
		m_minimumLevel = level;
	}

	pulsar::Log::Level Log::getMinimumLevel() noexcept
	{
		return m_minimumLevel;
	}

	std::string Log::getDateTime() noexcept
	{
		// Time code. Simple.
		const std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string out = std::ctime(&time);
		out.erase(std::remove(out.begin(), out.end(), '\n'), out.end());
		return std::move(out);
	}

	std::string Log::processLevel(const pulsar::Log::Level level) noexcept
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

	std::string Log::processColour(pulsar::Log::Level level) noexcept
	{
		std::string out = "";

		switch (level)
		{
		case PL_INFO:
			out = pulsar::colourText(LogColours::WHITE);
			break;

		case PL_DEBUG:
			out = pulsar::colourText(LogColours::GREEN);
			break;

		case PL_WARNING:
			out = pulsar::colourText(LogColours::YELLOW);
			break;

		case PL_ERROR:
			out = pulsar::colourText(LogColours::RED);
			break;

		case PL_FATAL:
			out = pulsar::colourText(LogColours::FATAL);
			break;

		default:
			out = pulsar::colourText(LogColours::WHITE);
			break;
		}

		return std::move(out);
	}

	bool Log::filterLevel(pulsar::Log::Level level) noexcept
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
}