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
#define LOG_S(x) pl::Log::s_stream << pl::Log::processColour(pl::Log::Level::x) << "[" << pl::Log::processLevel(pl::Log::Level::x) << "] - " << pl::Log::getDateTime() << " - "

namespace pl
{
	struct Log
	{
		enum class Level
		{
			INFO,
			DEBUG,
			WARNING,
			ERROR,
			FATAL
		};

		static inline void init(const std::string& logTo)
		{
			pl::Log::s_stream.init(logTo);
		}

		static inline std::string getDateTime()
		{
			std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::string out = std::ctime(&time);
			out.erase(std::remove(out.begin(), out.end(), '\n'), out.end());
			return out;
		}

		static inline std::string processLevel(pl::Log::Level level)
		{
			std::string out = "";

			switch (level)
			{
			case Level::INFO:
				out = "INFO";
				break;

			case Level::DEBUG:
				out = "DEBUG";
				break;

			case Level::WARNING:
				out = "WARNING";
				break;

			case Level::ERROR:
				out = "ERROR";
				break;

			case Level::FATAL:
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
			case Level::INFO:
				out = Platform::colourText(LogColours::WHITE);
				break;

			case Level::DEBUG:
				out = Platform::colourText(LogColours::GREEN);
				break;

			case Level::WARNING:
				out = Platform::colourText(LogColours::YELLOW);
				break;

			case Level::ERROR:
				out = Platform::colourText(LogColours::RED);
				break;

			case Level::FATAL:
				out = Platform::colourText(LogColours::FATAL);
				break;

			default:
				out = Platform::colourText(LogColours::WHITE);
				break;
			}

			return out;
		}

		static inline pl::LogStream s_stream = {};
	};
}

#endif