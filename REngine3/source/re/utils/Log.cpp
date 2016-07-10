//
//  Log.cpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <fstream>
#include <iostream>

#ifdef _WIN32
#include "re/platform/Win32Log.hpp"
#else
#include "re/platform/POSIXLog.hpp"
#endif
#include "re/utils/Time.hpp"

#include "Log.hpp"

// Yellow: FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
// Red: FOREGROUND_RED | FOREGROUND_INTENSITY
// Gray: FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN

namespace re
{
	Log& Log::Get()
	{
		static Log pa;
		return pa;
	}

	Log::~Log()
	{
		if (m_enableFileWriting)
		{
			std::ofstream out;
			std::string file = "logs/[" + GetCurrentTimeAndDate() + "] - REngine3.log";
			out.open(file);

			for (auto& v : m_loggedMessages)
			{
				out << v << std::endl;
			}

			out.close();
		}

		m_loggedMessages.clear();
	}

	void Log::EnableLogging(bool isEnabled)
	{
		m_enableLogging = isEnabled;
	}

	void Log::EnableFileWriting(bool isEnabled)
	{
		m_enableFileWriting = isEnabled;
	}

	void Log::LogMessage(int level, const std::string & message)
	{
		if (m_enableLogging)
		{
			switch (level)
			{
			case LogLevel::INFO:
				std::cout << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN) << "RE_INFO:    [" << GetCurrentTimeAndDate() << "] - " << message << "." << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN) << std::endl;
				break;

			case LogLevel::WARNING:
				std::cout << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY) << "RE_WARNING:    [" << GetCurrentTimeAndDate() << "] - " << message << "." << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN) << std::endl;
				break;

			case LogLevel::FATAL:
				std::cout << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_INTENSITY) << "RE_ERROR:      [" << GetCurrentTimeAndDate() << "] - " << message << "." << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN) << std::endl;
				break;

			default:
				std::cout << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN) << "RE_INFO:    [" << GetCurrentTimeAndDate() << "] - " << message << "." << SetConsoleTextColour(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN) << std::endl;
				break;
			}
		}
	}
}