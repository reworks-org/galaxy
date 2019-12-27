///
/// Windows.cpp
///
/// pulsar
/// See LICENSE.txt.
///

#if defined(_WIN64) || defined(_WIN32)

#include <Windows.h>

#include "Windows.hpp"

///
/// Core namespace.
///
namespace pl
{
	const std::string colourText(const LogColours colour)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, (int)colour);

		return "";
	}
}

#endif