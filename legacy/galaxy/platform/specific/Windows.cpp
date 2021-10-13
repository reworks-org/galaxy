///
/// Windows.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include "Windows.hpp"

#ifdef GALAXY_WIN_PLATFORM

#include <Windows.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/error/Log.hpp"

namespace galaxy
{
	namespace platform
	{
		void configure_terminal() noexcept
		{
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD  mode   = 0;

			GetConsoleMode(handle, &mode);
			mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			SetConsoleMode(handle, mode);
		}
	} // namespace platform
} // namespace galaxy

#endif