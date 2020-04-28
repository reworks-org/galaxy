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
namespace pulsar
{
	std::string colourText(const LogColours colour) noexcept
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, (int)colour);

		return "";
	}
}

#endif