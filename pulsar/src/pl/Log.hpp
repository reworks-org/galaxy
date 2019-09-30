///
/// Log.hpp
///
/// pulsar
/// See LICENSE.txt.
///

#ifndef PULSAR_LOG_HPP_
#define PULSAR_LOG_HPP_

#include "platform/Unix.hpp"
#include "platform/Windows.hpp"
#include "details/LogStream.hpp"

///
/// Log to Stream.
///
#define LOG_S(x) pl::Log::filterLevel(x) << /*pl::Log::processColour(x) <<*/ "[" << pl::Log::processLevel(x) << "] - " << pl::Log::getDateTime() << " - "


namespace pl
{
	class Log
	{
	public:
		enum class Level : int
		{
			INFO = 0,
			DEBUG = 1,
			WARNING = 2,
			ERROR = 3,
			FATAL = 4
		};

		static inline void init(const std::string& logTo)
		{
			pl::Log::s_stream.init(logTo);
		}

		static inline std::string processLevel(pl::Log::Level level)
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

		static inline std::string processColour(pl::Log::Level level)
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

		static inline pl::LogStream& filterLevel(pl::Log::Level level)
		{
			if (static_cast<int>(level) >= static_cast<int>(pl::Log::s_minimumLevel))
			{
				return pl::Log::s_stream;
			}
			else
			{
				return pl::Log::s_emptyStream;
			}
		}

		static inline void setMinimumLevel(pl::Log::Level level)
		{
			pl::Log::s_minimumLevel = level;
		}

		static inline pl::Log::Level getMinimumLevel()
		{
			return pl::Log::s_minimumLevel;
		}

		static inline std::string getDateTime()
		{
			std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::string out = std::ctime(&time);
			out.erase(std::remove(out.begin(), out.end(), '\n'), out.end());
			return out;
		}

	private:
		static inline pl::LogStream s_stream = pl::LogStream(false);
		static inline pl::LogStream s_emptyStream = pl::LogStream(true);
		static inline pl::Log::Level s_minimumLevel;
	};
}

#endif