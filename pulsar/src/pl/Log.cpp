///
/// Log.cpp
///
/// pulsar
/// See LICENSE.txt.
///

#include <ctime>
#include <chrono>
#include <future>
#include <iostream>
#include <algorithm>
#include <filesystem>

#include "Log.hpp"

///
/// Core namespace.
///
namespace pl
{
	Log::Log() noexcept
		:m_minimumLevel(pl::Log::Level::INFO)
	{
		m_callback = [&](const pl::Log::Level level, const std::string& message)
		{
			// Mutex protection.
			m_lock.lock();

			// Prefix string
			std::string output = pl::Log::i().processColour(level) + "[" + pl::Log::i().processLevel(level) + "] - " + pl::Log::i().getDateTime() + " - ";
			output += (message + "\n");

			// Print to stream and std output.
			std::cout << output;
			m_fileStream << output;

			m_lock.unlock();
		};
	}

	Log::~Log() noexcept
	{
		m_fileStream.close();
	}

	Log& Log::i()
	{
		// Singleton instance.
		static Log s_inst;
		return s_inst;
	}

	void Log::init(const std::string& logTo)
	{
		// Find path
		std::filesystem::path path(logTo);
		std::filesystem::path directory = path.remove_filename();

		// Make sure it exists.
		if (!std::filesystem::exists(directory))
		{
			std::filesystem::create_directory(directory);
		}

		// Open and make sure its not errored.
		m_fileStream.open(logTo, std::ofstream::out);
		if (m_fileStream.fail())
		{
			throw std::runtime_error("Failed to create log: " + logTo);
		}
	}

	void Log::log(const pl::Log::Level level, const std::string& message)
	{
		// Check to make sure level should be logged.
		if (pl::Log::i().filterLevel(level))
		{
			// Launch thread to log to.
			std::thread thread(m_callback, level, message);
			thread.detach();
		}
	}

	std::string Log::processLevel(const pl::Log::Level level)
	{
		std::string out = "";

		switch (level)
		{
		case pl::Log::Level::INFO:
			out = "INFO";
			break;

		case pl::Log::Level::DEBUG:
			out = "DEBUG";
			break;

		case pl::Log::Level::WARNING:
			out = "WARNING";
			break;

		case pl::Log::Level::ERROR:
			out = "ERROR";
			break;

		case pl::Log::Level::FATAL:
			out = "FATAL";
			break;

		default:
			out = "INVALID MESSAGE LEVEL";
			break;
		}

		return out;
	}

	std::string Log::processColour(pl::Log::Level level)
	{
		std::string out = "";

		switch (level)
		{
		case pl::Log::Level::INFO:
			out = pl::colourText(LogColours::WHITE);
			break;

		case pl::Log::Level::DEBUG:
			out = pl::colourText(LogColours::GREEN);
			break;

		case pl::Log::Level::WARNING:
			out = pl::colourText(LogColours::YELLOW);
			break;

		case pl::Log::Level::ERROR:
			out = pl::colourText(LogColours::RED);
			break;

		case pl::Log::Level::FATAL:
			out = pl::colourText(LogColours::FATAL);
			break;

		default:
			out = pl::colourText(LogColours::WHITE);
			break;
		}

		return out;
	}

	bool Log::filterLevel(pl::Log::Level level)
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

	void Log::setMinimumLevel(pl::Log::Level level)
	{
		m_minimumLevel = level;
	}

	pl::Log::Level Log::getMinimumLevel()
	{
		return m_minimumLevel;
	}

	std::string Log::getDateTime()
	{
		// Time code. Simple.
		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string out = std::ctime(&time);
		out.erase(std::remove(out.begin(), out.end(), '\n'), out.end());
		return out;
	}
}