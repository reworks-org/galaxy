///
/// Windows.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Windows.hpp"

#ifdef GALAXY_WIN_PLATFORM

#include <Windows.h>

namespace galaxy
{
	namespace platform
	{
		void configure_terminal()
		{
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD  mode   = 0;

			if (handle != nullptr)
			{
				GetConsoleMode(handle, &mode);
				SetConsoleMode(handle, mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING);
			}
		}
	} // namespace platform
} // namespace galaxy

#endif
