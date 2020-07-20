///
/// Windows.cpp
///
/// pulsar
/// See LICENSE.txt.
///

#if defined(_WIN64) || defined(_WIN32)

#include <Windows.h>
#undef ERROR

#include "Windows.hpp"

///
/// Core namespace.
///
namespace pl
{
	std::string colourText(const LogColours colour) noexcept
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, static_cast<WORD>(colour));

		return "";
	}
} // namespace pulsar

#endif