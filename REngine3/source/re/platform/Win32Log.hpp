//
//  Windows.hpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_WINDOWS_HPP_
#define RENGINE3_WINDOWS_HPP_

#ifdef _WIN32
#include <Windows.h>

namespace re
{
	/*
	* IMPORTS: code - color code to use.
	* EXPORTS: none
	* PURPOSE: Change text colour of the console.
	*/
	const char* SetConsoleTextColour(unsigned long code)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, (WORD)code);
		return "";
	}
}
#endif

#endif