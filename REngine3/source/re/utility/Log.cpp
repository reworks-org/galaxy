//
//  Log.cpp
//  REngine3
//
//  Created by reworks on 17/04/2017.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <fstream>
#include <iostream>

#ifdef _WIN32
#include "re/platform/Win32Log.hpp"
#else
#include "re/platform/POSIXLog.hpp"
#endif

#include "re/utility/Time.hpp"

#include "Log.hpp"

#define YELLOW setConsoleTextColour(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define RED setConsoleTextColour(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define GRAY setConsoleTextColour(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)

namespace re
{
	Log::Log()
	{
	}

	Log& Log::instance()
	{
		static Log instance;
		return instance;
	}

	Log::~Log()
	{
		std::ofstream m_outputLogFile;
		std::string logFileName = "logs/" + Time::getFormattedTime() + ".log";
		
		m_outputLogFile.open(logFileName);

		for (size_t i = 0, ilen = m_savedMessages.size(); i < ilen; ++i)
		{
			m_outputLogFile << m_savedMessages[i] << std::endl;
		}

		m_outputLogFile.close();
		m_savedMessages.clear();
	}

	void Log::log(LogLevel level, const std::string& message, const std::string& function, const std::string& file, int line)
	{
		std::string composedMessage = Time::getCurrentTimeAndDate() + "] - Message: " + message + ". Function: " + function + ". File: " + file + ". Line: " + std::to_string(line) + ".";
		
		switch (level)
		{
		case LogLevel::INFO:
			std::cout << GRAY << "RE_INFO:    [" << composedMessage << GRAY << std::endl;
			m_savedMessages.push_back("RE_INFO:    [" + composedMessage);
			break;

		case LogLevel::WARNING:
			std::cout << YELLOW << "RE_WARNING: [" << composedMessage << GRAY << std::endl;
			m_savedMessages.push_back("RE_WARNING: [" + composedMessage);
			break;

		case LogLevel::FATAL:
			std::cout << RED << "RE_ERROR:   [" << composedMessage << GRAY << std::endl;
			m_savedMessages.push_back("RE_ERROR:   [" + composedMessage);
			break;
		}
	}

	void Log::printPrettyText(LogLevel level, const std::string& message)
	{
		std::string top = "";
		std::string middle = "";
		std::string bottom = "";

		for (size_t i = 0, ilen = message.length(); i < (ilen + 6); ++i)
		{
			top += "*";
		}

		bottom = top;
		middle = "*  " + message + "  *";
		
		switch (level)
		{
		case LogLevel::INFO:
			std::cout << GRAY << top << GRAY << std::endl;
			std::cout << GRAY << middle << GRAY << std::endl;
			std::cout << GRAY << bottom << GRAY << std::endl;

			m_savedMessages.push_back(top);
			m_savedMessages.push_back(middle);
			m_savedMessages.push_back(bottom);
			break;

		case LogLevel::WARNING:
			std::cout << YELLOW << top << GRAY << std::endl;
			std::cout << YELLOW << middle << GRAY << std::endl;
			std::cout << YELLOW << bottom << GRAY << std::endl;

			m_savedMessages.push_back(top);
			m_savedMessages.push_back(middle);
			m_savedMessages.push_back(bottom);
			break;

		case LogLevel::FATAL:
			std::cout << RED << top << GRAY << std::endl;
			std::cout << RED << middle << GRAY << std::endl;
			std::cout << RED << bottom << GRAY << std::endl;

			m_savedMessages.push_back(top);
			m_savedMessages.push_back(middle);
			m_savedMessages.push_back(bottom);
			break;
		}
	}
}