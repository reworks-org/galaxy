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
namespace pulsar
{
	Log::Log() noexcept
		:m_minimumLevel(pulsar::Log::Level::INFO)
	{
		m_callback = [&](const pulsar::Log::Level level, const std::string& message)
		{
			// Prefix string
			std::string output = pulsar::Log::get().processColour(level) + "[" + pulsar::Log::get().processLevel(level) + "] - " + pulsar::Log::get().getDateTime() + " - ";
			output += (message + "\n");

			// Mutex protection.
			m_lock.lock();

			// Print to stream and std output.
			std::cout << output;
			m_fileStream << output;

			m_lock.unlock();
		};
	}

	Log::~Log() noexcept
	{
		deinit();
	}

	Log& Log::get()
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

	void Log::deinit() noexcept
	{
		m_lock.lock();
		m_fileStream.close();
		m_lock.unlock();

		// Make sure each logging thread has finished before shutting down.
		for (auto& future : m_futures)
		{
			future.get();
		}
	}

	void Log::log(const pulsar::Log::Level level, const std::string& message)
	{
		// Check to make sure level should be logged.
		if (pulsar::Log::get().filterLevel(level))
		{
			// Launch thread to log to.
			m_futures.emplace_back(std::move(std::async(std::launch::async, m_callback, level, message)));
		}
	}

	std::string Log::processLevel(const pulsar::Log::Level level)
	{
		std::string out = "";

		switch (level)
		{
		case pulsar::Log::Level::INFO:
			out = "INFO";
			break;

		case pulsar::Log::Level::DEBUG:
			out = "DEBUG";
			break;

		case pulsar::Log::Level::WARNING:
			out = "WARNING";
			break;

		case pulsar::Log::Level::ERROR_:
			out = "ERROR";
			break;

		case pulsar::Log::Level::FATAL:
			out = "FATAL";
			break;

		default:
			out = "INVALID MESSAGE LEVEL";
			break;
		}

		return out;
	}

	std::string Log::processColour(pulsar::Log::Level level)
	{
		std::string out = "";

		switch (level)
		{
		case pulsar::Log::Level::INFO:
			out = pulsar::colourText(LogColours::WHITE);
			break;

		case pulsar::Log::Level::DEBUG:
			out = pulsar::colourText(LogColours::GREEN);
			break;

		case pulsar::Log::Level::WARNING:
			out = pulsar::colourText(LogColours::YELLOW);
			break;

		case pulsar::Log::Level::ERROR_:
			out = pulsar::colourText(LogColours::RED);
			break;

		case pulsar::Log::Level::FATAL:
			out = pulsar::colourText(LogColours::FATAL);
			break;

		default:
			out = pulsar::colourText(LogColours::WHITE);
			break;
		}

		return out;
	}

	bool Log::filterLevel(pulsar::Log::Level level)
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

	void Log::setMinimumLevel(pulsar::Log::Level level)
	{
		m_minimumLevel = level;
	}

	pulsar::Log::Level Log::getMinimumLevel()
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