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

#define RE_LOG_ENABLE(boolean) re::Log::Get().EnableLogging(boolean) 
#define RE_LOG_ENABLE_FILE(boolean) re::Log::Get().EnableFileWriting(boolean)
#define RE_LOG(level, message) re::Log::Get().LogMessage(level, message)

#define RE_ASSERT(x, messageFileLine) \
		if (!(x)) {\
			RE_LOG(LogLevel::FATAL, "*************************"); \
			RE_LOG(LogLevel::FATAL, "    ASSERTION FAILED!    "); \
			RE_LOG(LogLevel::FATAL, "*************************"); \
			RE_LOG(LogLevel::FATAL, messageFileLine); \
}

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