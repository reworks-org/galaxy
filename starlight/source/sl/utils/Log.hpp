///
/// Log.hpp
/// starlight
///
/// Created by reworks on 2/07/2019.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_LOG_HPP_
#define STARLIGHT_LOG_HPP_

#include <fstream>
#include <iostream>

#define LOG_S(level, message) sl::Log::stream(level, message);

namespace sl
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