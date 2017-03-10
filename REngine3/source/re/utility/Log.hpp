//
//  Log.hpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_LOG_HPP_
#define RENGINE3_LOG_HPP_

#include <vector>
#include <string>

namespace re
{
	enum LogLevel
	{
		INFO,
		WARNING,
		FATAL
	};

	static bool g_enableLog = false;
	static std::vector<std::string> g_loggedMessages;

	inline void log(LogLevel level, const std::string& message)
	{
		
	}
}

/*
* IMPORTS: boolean - true / false value
* EXPORTS: none
* PURPOSE: Enable or disable logging.
*/
#define RE_LOG_ENABLE(boolean) re::Log::Get().EnableLogging(boolean) 

/*
* IMPORTS: boolean - true / false value
* EXPORTS: none
* PURPOSE: Enable or disable writing log to file.
*/
#define RE_LOG_ENABLE_FILE(boolean) re::Log::Get().EnableFileWriting(boolean)

/*
* IMPORTS: level - enum of log level, message - the message to log.
* EXPORTS: none
* PURPOSE: Output a message in a logged format for debugging purposes.
*/
#define RE_LOG(level, message) re::Log::Get().LogMessage(level, message)

/*
* IMPORTS: x - value to compare, messageFileLine - the file, line and message to log.
* EXPORTS: none
* PURPOSE: Assert using our logging system.
*/
#define RE_ASSERT(x, messageFileLine) \
		if (!(x)) {\
			RE_LOG(LogLevel::FATAL, "*************************"); \
			RE_LOG(LogLevel::FATAL, "    ASSERTION FAILED!    "); \
			RE_LOG(LogLevel::FATAL, "*************************"); \
			RE_LOG(LogLevel::FATAL, messageFileLine); \
			throw std::runtime_error(); \
		}

/*
* Singleton crap. Dont even look...
*/
namespace re
{
	enum LogLevel
	{
		
	};

	class Log
	{
	public:
		static Log& Get();
		~Log();

		void EnableLogging(bool isEnabled);
		void EnableFileWriting(bool isEnabled);

		void LogMessage(int level, const std::string& message);

	private:
		inline Log() {}
		Log(Log const&) = delete;
		void operator=(Log const&) = delete;

		bool m_enableLogging = true;
		bool m_enableFileWriting = false;
		std::vector<std::string> m_loggedMessages;
	};
}

#endif