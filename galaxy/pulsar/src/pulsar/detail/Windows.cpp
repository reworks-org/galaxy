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
	std::string colour_text(const LogColours colour) noexcept
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(h, static_cast<WORD>(colour));

		return {};
	}
} // namespace pl

#endif