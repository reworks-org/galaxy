///
/// Log.hpp
///
/// pulsar
/// See LICENSE.txt.
///

#ifndef PULSAR_LOG_HPP_
#define PULSAR_LOG_HPP_

#include "LogStream.hpp"


///
/// Log to Stream.
///
#define LOG_S(level) pl::Log::s_stream << "[" << level << "] - " << pl::Log::getDateTime() << " - "

namespace pl
{
	struct Log
	{
		static inline constexpr auto DEBUG = "DEBUG";
		static inline constexpr auto INFO = "INFO";
		static inline constexpr auto WARNING = "WARNING";
		static inline constexpr auto ERROR = "ERROR";
		static inline constexpr auto FATAL = "FATAL";
		static inline pl::LogStream s_stream = {};

		static inline std::string getDateTime()
		{
			return "";
		}

		static inline void init(const std::string& logTo)
		{
			pl::Log::s_stream.init(logTo);
		}
	};
}

#endif