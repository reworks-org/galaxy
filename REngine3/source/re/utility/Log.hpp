//
//  Log.hpp
//  REngine3
//
//  Created by reworks on 17/04/2017.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_LOG_HPP_
#define RENGINE3_LOG_HPP_

#include <vector>
#include <string>

// Convenience Macros
#ifndef NDEBUG

#define RE_LOG(_LEVEL_, _MESSAGE_, _FUNCTION_, _FILE_, _LINE_) re::Log::instance().log(_LEVEL_, _MESSAGE_, _FUNCTION_, _FILE_, _LINE_)
#define RE_LOG_PRINTPRETTY(_LEVEL_, _MESSAGE_) re::Log::instance().printPrettyText(_LEVEL_, _MESSAGE_)
#define RE_LOG_SAVE(_VALUE_) re::Log::instance().saveToLog(_VALUE_)

#define RE_ASSERT(_VALUE_, _MESSAGE_, _FUNCTION_, _FILE_, _LINE_) \
		if (!(_VALUE_)) \
        { \
			RE_LOG_PRINTPRETTY(re::LogLevel::FATAL, "ASSERTION FAILED!"); \
			RE_LOG(re::LogLevel::FATAL, _MESSAGE_, _FUNCTION_, _FILE_, _LINE_); \
			throw std::runtime_error("Refer to console or log file for details!"); \
		}

#define RE_REVERSE_ASSERT(_VALUE_, _MESSAGE_, _FUNCTION_, _FILE_, _LINE_) \
		if ((_VALUE_)) \
        { \
			RE_LOG_PRINTPRETTY(re::LogLevel::FATAL, "ASSERTION FAILED!"); \
			RE_LOG(re::LogLevel::FATAL, _MESSAGE_, _FUNCTION_, _FILE_, _LINE_); \
			throw std::runtime_error("Refer to console or log file for details!"); \
		}

#define RE_ASSERT_COMPARE(_VALUEA_, _VALUEB_, _MESSAGE_, _FUNCTION_, _FILE_, _LINE_) \
		if ((_VALUEA_) != (_VALUEB_)) \
        { \
			RE_LOG_PRINTPRETTY(re::LogLevel::FATAL, "ASSERTION FAILED!"); \
			RE_LOG(re::LogLevel::FATAL, _MESSAGE_, _FUNCTION_, _FILE_, _LINE_); \
			throw std::runtime_error("Refer to console or log file for details!"); \
		}

#define RE_REVERSE_ASSERT_COMPARE(_VALUEA_, _VALUEB_, _MESSAGE_, _FUNCTION_, _FILE_, _LINE_) \
		if ((_VALUEA_) == (_VALUEB_)) \
        { \
			RE_LOG_PRINTPRETTY(re::LogLevel::FATAL, "ASSERTION FAILED!"); \
			RE_LOG(re::LogLevel::FATAL, _MESSAGE_, _FUNCTION_, _FILE_, _LINE_); \
			throw std::runtime_error("Refer to console or log file for details!"); \
		}
#else

#define RE_LOG(_LEVEL_, _MESSAGE_, _FUNCTION_, _FILE_, _LINE_)
#define RE_LOG_PRINTPRETTY(_LEVEL_, _MESSAGE_)
#define RE_LOG_SAVE(_VALUE_)
#define RE_ASSERT(_VALUE_, _MESSAGE_, _FUNCTION_, _FILE_, _LINE_)
#define RE_REVERSE_ASSERT(_VALUE_, _MESSAGE_, _FUNCTION_, _FILE_, _LINE_)
#define RE_ASSERT_COMPARE(_VALUEA_, _VALUEB_, _MESSAGE_, _FUNCTION_, _FILE_, _LINE_)
#define RE_REVERSE_ASSERT_COMPARE(_VALUEA_, _VALUEB_, _MESSAGE_, _FUNCTION_, _FILE_, _LINE_)

#endif

namespace re
{
	enum LogLevel
	{
		INFO,
		WARNING,
		FATAL
	};

	class Log
	{
	public:
		Log(Log const&) = delete;
		Log(Log&&) = delete;
		Log& operator=(Log const&) = delete;
		Log& operator=(Log &&) = delete;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Returns Singleton instance of class.
		*/
		static Log& instance();

		/*
		* IMPORTS: level - enum of log level, message - the message to log, file - name of file, line - line number (vs2017)
		* EXPORTS: none
		* PURPOSE: Output a message in a logged format for debugging purposes by overloading the () operator.
		*/
		void log(LogLevel level, const std::string& message, const std::string& function, const std::string& file, int line);

		/*
		* IMPORTS: message to print
		* EXPORTS: none
		* PURPOSE: Prints a message to console with astericks around it.
		*/
		void printPrettyText(LogLevel level, const std::string& message);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Saves logged information to a physical file by timestamp.
		*/
		void saveToLog(bool saveLog);

	protected:
		Log();
		~Log();

	private:
		std::vector<std::string> m_savedMessages;
	};
}

#endif
