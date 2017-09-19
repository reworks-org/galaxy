//
//  Log.hpp
//  REngine3
//
//  Created by reworks on 17/04/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_LOG_HPP_
#define RENGINE3_LOG_HPP_

#include <vector>
#include <string>

#include "re/graphics/Window.hpp"

#define RE_LOG_PROVIDEWINDOW(_WINDOW_) re::Log::instance().provideWindow(_WINDOW_)
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

// Horrible static class implementation...
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

		static Log& instance();
		void provideWindow(Window* window);
		void log(LogLevel level, const std::string& message, const std::string& function, const std::string& file, int line);
		void printPrettyText(LogLevel level, const std::string& message);
		void saveToLog(bool saveLog);
	protected:
		Log();
		~Log();
	private:
		std::vector<std::string> m_savedMessages;
		Window* m_windowRef;
	};
}

#endif
