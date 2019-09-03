///
/// Log.hpp
///
/// pulsar
/// See LICENSE.txt.
///

#ifndef PULSAR_LOG_HPP_
#define PULSAR_LOG_HPP_

#include <iostream>
#include <fstream>

///
/// Log to Stream.
///
#define LOG_S(level) pl::Log::STREAM << "[" << pl::Log::level << "] - " << pl::Log::getDateTime() << " - "

namespace pl
{
	struct Log
	{
		struct Stream
		{
			template<typename T>
			friend inline decltype(auto) operator<<(std::ostream& os, const T& type)
			{
				return os;
			}
		};

		static inline auto constexpr DEBUG = "DEBUG";
		static inline auto constexpr INFO = "INFO";
		static inline auto constexpr WARNING = "WARNING";
		static inline auto constexpr ERROR = "ERROR";
		static inline auto constexpr FATAL = "FATAL";
		static inline pl::Log::Stream constexpr s_stream;

		static inline std::string getDateTime()
		{

		}

		static inline init(const std::string& logTo)
		{
			std::ofstream log;
			log.open(logTo);
			if (log.fail())
			{
				throw std::runtime_error("Failed to create log: " + logTo);
			}
		}

		

		static inline log(Level level, const std::string& message)
		{

		}
	};
}

#endif