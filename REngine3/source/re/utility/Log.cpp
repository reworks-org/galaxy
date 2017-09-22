//
//  Log.cpp
//  REngine3
//
//  Created by reworks on 17/04/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <fstream>
#include <iostream>

#include <allegro5/allegro_native_dialog.h>

#ifdef _WIN32
#include "re/platform/Win32Log.hpp"
#else
#include "re/platform/UNIXLog.hpp"
#endif

#include "re/types/Colour.hpp"
#include "re/utility/Time.hpp"

#include "Log.hpp"

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
	}

	void Log::provideWindow(Window* window)
	{
		m_windowRef = window;
	}

	void Log::log(LogLevel level, const std::string& message, const std::string& function, const std::string& file, int line)
	{
		std::string composedMessage = Time::getCurrentTimeAndDate() + "] - Message: " + message + ". Function: " + function + ". File: " + file + ". Line: " + std::to_string(line) + ".";
		
		switch (level)
		{
		case LogLevel::INFO:
			std::cout << setConsoleTextColour(RE_GRAY) << "RE_INFO:    [" << composedMessage << setConsoleTextColour(RE_GRAY) << std::endl;
			m_savedMessages.push_back("RE_INFO:    [" + composedMessage);
			break;

		case LogLevel::WARNING:
			std::cout << setConsoleTextColour(RE_YELLOW) << "RE_WARNING: [" << composedMessage << setConsoleTextColour(RE_GRAY) << std::endl;
			m_savedMessages.push_back("RE_WARNING: [" + composedMessage);
			break;

		case LogLevel::FATAL:
			std::cout << setConsoleTextColour(RE_RED) << "RE_ERROR:   [" << composedMessage << setConsoleTextColour(RE_GRAY) << std::endl;
			m_savedMessages.push_back("RE_ERROR:   [" + composedMessage);

			al_show_native_message_box(m_windowRef->getDisplay(), "ERROR", nullptr, message.c_str(), nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			m_windowRef->close();
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
			std::cout << setConsoleTextColour(RE_GRAY) << top << setConsoleTextColour(RE_GRAY) << std::endl;
			std::cout << setConsoleTextColour(RE_GRAY) << middle << setConsoleTextColour(RE_GRAY) << std::endl;
			std::cout << setConsoleTextColour(RE_GRAY) << bottom << setConsoleTextColour(RE_GRAY) << std::endl;

			m_savedMessages.push_back(top);
			m_savedMessages.push_back(middle);
			m_savedMessages.push_back(bottom);
			break;

		case LogLevel::WARNING:
			std::cout << setConsoleTextColour(RE_YELLOW) << top << setConsoleTextColour(RE_GRAY) << std::endl;
			std::cout << setConsoleTextColour(RE_YELLOW) << middle << setConsoleTextColour(RE_GRAY) << std::endl;
			std::cout << setConsoleTextColour(RE_YELLOW) << bottom << setConsoleTextColour(RE_GRAY) << std::endl;

			m_savedMessages.push_back(top);
			m_savedMessages.push_back(middle);
			m_savedMessages.push_back(bottom);
			break;

		case LogLevel::FATAL:
			std::cout << setConsoleTextColour(RE_RED) << top << setConsoleTextColour(RE_GRAY) << std::endl;
			std::cout << setConsoleTextColour(RE_RED) << middle << setConsoleTextColour(RE_GRAY) << std::endl;
			std::cout << setConsoleTextColour(RE_RED) << bottom << setConsoleTextColour(RE_GRAY) << std::endl;

			m_savedMessages.push_back(top);
			m_savedMessages.push_back(middle);
			m_savedMessages.push_back(bottom);

			al_show_native_message_box(m_windowRef->getDisplay(), "ERROR", nullptr, message.c_str(), nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			m_windowRef->close();
			break;
		}
	}

	void Log::saveToLog(bool saveLog)
	{
		if (saveLog)
		{
			std::ofstream m_outputLogFile;
			std::string logFileName = Time::getFormattedTime() + ".log";

			m_outputLogFile.open(logFileName);
			RE_REVERSE_ASSERT(m_outputLogFile.fail(), "Failed to open log file for saving!", "Log::saveToLog", "Log.cpp", 122);

			for (size_t i = 0, ilen = m_savedMessages.size(); i < ilen; ++i)
			{
				m_outputLogFile << m_savedMessages[i] << std::endl;
			}

			m_outputLogFile.close();
			m_savedMessages.clear();
		}
	}
}