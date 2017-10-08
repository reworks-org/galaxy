//
//  Windows.hpp
//  rework
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_WIN32LOG_HPP_
#define REWORK_WIN32LOG_HPP_

#ifdef _WIN32
#include <Windows.h>

namespace re
{
	///
	/// Change the colour of the text being printed to the console.
	///
	/// \param code ACSII RE_<COLOUR> code to use.
	///
	inline const char* setConsoleTextColour(unsigned long code)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, (WORD)code);
		return "";
	}
}
#endif

#endif