///
/// Log.hpp
/// galaxy
///
/// Created by reworks on 2/07/2019.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LOG_HPP_
#define GALAXY_LOG_HPP_

#include <fstream>
#include <iostream>

#define LOG_S(level, message) galaxy::Log::stream(level, message);

namespace galaxy
{
	enum class LogLevel
	{
		INFO,
		WARNING,
		ERROR,
		FATAL
	};

	class Log
	{
	public:
		static inline void stream(LogLevel level, std::string_view message)
		{
			switch (level)
			{
			case LogLevel::INFO:
				break;

			case LogLevel::WARNING:
				break;

			case LogLevel::ERROR:
				break;

			case LogLevel::FATAL:
				break;
			}
		}

	private:
		Log() = delete;

		static inline void logFatal(std::string_view message)
		{

		}
	}
}

#endif