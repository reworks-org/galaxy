///
/// Windows.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifdef _WIN32 || _WIN64

#include <Windows.h>

#include "galaxy/error/Log.hpp"

#include "Windows.hpp"

namespace galaxy
{
	namespace platform
	{
		void configure_terminal()
		{
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD mode    = 0;

			GetConsoleMode(handle, &mode);
			mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			SetConsoleMode(handle, mode);
		}

		void run_process(std::string_view path)
		{
			const std::wstring wstr = std::wstring(path.begin(), path.end());

			STARTUPINFO startup_info = {sizeof(startup_info)};
			PROCESS_INFORMATION process_info;
			if (CreateProcess(wstr.c_str(), nullptr, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startup_info, &process_info))
			{
				WaitForSingleObject(process_info.hProcess, INFINITE);
				CloseHandle(process_info.hProcess);
				CloseHandle(process_info.hThread);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to start process: {0}.", path);
			}
		}
	} // namespace platform
} // namespace galaxy

#endif