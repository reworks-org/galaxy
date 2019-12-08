///
/// Log.cpp
///
/// pulsar
/// See LICENSE.txt.
///

#include "Log.hpp"

///
/// Core namespace.
///
namespace pl
{
	Log::Log()
		:m_stream(false), m_emptyStream(true), m_minimumLevel(pl::Log::Level::INFO)
	{
	}

	Log& Log::i()
	{
		// Singleton instance.
		static Log s_inst;
		return s_inst;
	}

	void Log::init(const std::string& logTo)
	{
		m_stream.init(logTo);
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
			out = Platform::colourText(LogColours::WHITE);
			break;

		case pl::Log::Level::DEBUG:
			out = Platform::colourText(LogColours::GREEN);
			break;

		case pl::Log::Level::WARNING:
			out = Platform::colourText(LogColours::YELLOW);
			break;

		case pl::Log::Level::ERROR:
			out = Platform::colourText(LogColours::RED);
			break;

		case pl::Log::Level::FATAL:
			out = Platform::colourText(LogColours::FATAL);
			break;

		default:
			out = Platform::colourText(LogColours::WHITE);
			break;
		}

		return out;
	}

	pl::LogStream& Log::filterLevel(pl::Log::Level level)
	{
		std::lock_guard<std::mutex> lock(m_lock);

		// Checks for proper stream level.
		if (static_cast<int>(level) >= static_cast<int>(m_minimumLevel))
		{
			return m_stream;
		}
		else
		{
			return m_emptyStream;
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
		std::lock_guard<std::mutex> lock(m_lock);

		// Time code. Simple.
		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string out = std::ctime(&time);
		out.erase(std::remove(out.begin(), out.end(), '\n'), out.end());
		return out;
	}

	pl::LogStream& Log::stream()
	{
		return m_stream;
	}
}