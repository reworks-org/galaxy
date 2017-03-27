//
//  Log.hpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

// Colour guide
// Yellow: FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
// Red: FOREGROUND_RED | FOREGROUND_INTENSITY
// Gray: FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN

#ifndef RENGINE3_LOG_HPP_
#define RENGINE3_LOG_HPP_

#include <vector>
#include <string>
#include <iostream>

#include "re/utility/Time.hpp"

#ifdef _WIN32
#include "re/platform/Win32Log.hpp"
#else
#include "re/platform/POSIXLog.hpp"
#endif

/*
* IMPORTS: x - value to compare, message - information to log.
* EXPORTS: none
* PURPOSE: Assert using our logging system.
*/
#define RE_ASSERT(value, message) \
		if (!(value)) \
        { \
			re::log(LogLevel::FATAL, "*************************"); \
			re::log(LogLevel::FATAL, "    ASSERTION FAILED!    "); \
			re::log(LogLevel::FATAL, "*************************"); \
			re::log(LogLevel::FATAL, message); \
			throw std::runtime_error(message); \
		}

/*
 * IMPORTS: valueA - first value, valueB - other value to compare, message - information to log.
 * EXPORTS: none
 * PURPOSE: IF valueA IS NOT EQUAL TO valueB, THROW ERROR
 */
#define RE_ASSERT_NOTEQUAL(valueA, valueB, message) \
        if ((valueA) != (valueB)) \
        { \
            re::log(LogLevel::FATAL, "*************************"); \
            re::log(LogLevel::FATAL, "    ASSERTION FAILED!    "); \
            re::log(LogLevel::FATAL, "*************************"); \
            re::log(LogLevel::FATAL, message); \
            throw std::runtime_error(message); \
        }

/*
 * IMPORTS: valueA - first value, valueB - other value to compare, message - information to log.
 * EXPORTS: none
 * PURPOSE: IF valueA IS EQUAL TO valueB, THROW ERROR
 */
#define RE_ASSERT_EQUAL(valueA, valueB, message) \
if ((valueA) == (valueB)) \
{ \
re::log(LogLevel::FATAL, "*************************"); \
re::log(LogLevel::FATAL, "    ASSERTION FAILED!    "); \
re::log(LogLevel::FATAL, "*************************"); \
re::log(LogLevel::FATAL, message); \
throw std::runtime_error(message); \
}

// Legacy macros, for classes and functions that use them.
#define RE_LOG(level, message) re::log(level, message)
#define RE_ENABLE_LOG(value) re::s_enableLogging = value

namespace re
{
	enum LogLevel
	{
		INFO,
		WARNING,
		FATAL
	};

	static bool s_enableLogging = false;

	/*
	* IMPORTS: level - enum of log level, message - the message to log.
	* EXPORTS: none
	* PURPOSE: Output a message in a logged format for debugging purposes.
	*/
	inline void log(LogLevel level, const std::string& message)
	{
		if (s_enableLogging)
		{
			switch (level)
			{
				case LogLevel::INFO:
					std::cout << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN) << "RE_INFO:    [" << Time::GetCurrentTimeAndDate() << "] - " << message << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN) << std::endl;
				break;

				case LogLevel::WARNING:
					std::cout << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY) << "RE_WARNING: [" << Time::GetCurrentTimeAndDate() << "] - " << message << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN) << std::endl;
					break;

				case LogLevel::FATAL:
					std::cout << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_INTENSITY) << "RE_ERROR:   [" << Time::GetCurrentTimeAndDate() << "] - " << message << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN) << std::endl;
					break;
				
				default:
					std::cout << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN) << "RE_INFO:    [" << Time::GetCurrentTimeAndDate() << "] - " << message << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN) << std::endl;
					break;
			}
		}
	}
}

#endif
