//
//  ConsoleColour.hpp
//  rework
//
//  Created by reworks on 22/09/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_CONSOLECOLOUR_HPP_
#define REWORK_CONSOLECOLOUR_HPP_

#ifdef _WIN32
	#include <Windows.h>
	
	#define RE_BLACK 0
	#define RE_DARKBLUE FOREGROUND_BLUE
	#define RE_DARKGREEN FOREGROUND_GREEN
	#define RE_DARKCYAN FOREGROUND_GREEN | FOREGROUND_BLUE
	#define RE_DARKRED FOREGROUND_RED
	#define RE_DARKMAGENTA FOREGROUND_RED | FOREGROUND_BLUE
	#define RE_DARKYELLOW FOREGROUND_RED | FOREGROUND_GREEN
	#define RE_DARKGRAY FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
	#define RE_GRAY FOREGROUND_INTENSITY
	#define RE_BLUE FOREGROUND_INTENSITY | FOREGROUND_BLUE
	#define RE_GREEN FOREGROUND_INTENSITY | FOREGROUND_GREEN
	#define RE_CYAN FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE
	#define RE_RED FOREGROUND_INTENSITY | FOREGROUND_RED
	#define RE_MAGENTA FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE
	#define RE_YELLOW FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN
	#define RE_WHITE FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#else
	#define RE_BLACK "\x1b[30m"
	#define RE_DARKBLUE "\x1b[34m"
	#define RE_DARKGREEN "\x1b[32m"
	#define RE_DARKCYAN "\x1b[36m"
	#define RE_DARKRED "\x1b[31m"
	#define RE_DARKMAGENTA "\x1b[35m"
	#define RE_DARKYELLOW "\x1b[33m"
	#define RE_DARKGRAY "\x1b[40m"
	#define RE_GRAY "\x1b[37m"
	#define RE_BLUE "\x1b[44m"
	#define RE_GREEN "\x1b[42m"
	#define RE_CYAN "\x1b[46m"
	#define RE_RED "\x1b[41m"
	#define RE_MAGENTA "\x1b[45m"
	#define RE_YELLOW "\x1b[43m"
	#define RE_WHITE "\x1b[47m"
#endif

#endif