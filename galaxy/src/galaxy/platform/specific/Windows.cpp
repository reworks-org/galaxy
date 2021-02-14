///
/// Windows.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifdef _WIN32 || _WIN64

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/error/Log.hpp"

#include "Windows.hpp"

// Thanks to: https://stackoverflow.com/a/20163705
namespace
{
	// Structure used to communicate data from and to enumeration procedure.
	struct EnumData
	{
		DWORD dwProcessId;
		HWND hWnd;
	};

	BOOL CALLBACK EnumProc(HWND hWnd, LPARAM lParam)
	{
		// Retrieve storage location for communication data
		EnumData& ed      = *(EnumData*)lParam;
		DWORD dwProcessId = 0x0;
		// Query process ID for hWnd
		GetWindowThreadProcessId(hWnd, &dwProcessId);
		// Apply filter - if you want to implement additional restrictions,
		// this is the place to do so.
		if (ed.dwProcessId == dwProcessId)
		{
			// Found a window matching the process ID
			ed.hWnd = hWnd;
			// Report success
			SetLastError(ERROR_SUCCESS);
			// Stop enumeration
			return FALSE;
		}
		// Continue enumeration
		return TRUE;
	}

	// Main entry
	HWND FindWindowFromProcessId(DWORD dwProcessId)
	{
		EnumData ed = {dwProcessId};
		if (!EnumWindows(EnumProc, (LPARAM)&ed) &&
		    (GetLastError() == ERROR_SUCCESS))
		{
			return ed.hWnd;
		}
		return NULL;
	}

	// Helper method for convenience.
	HWND FindWindowFromProcess(HANDLE hProcess)
	{
		return FindWindowFromProcessId(GetProcessId(hProcess));
	}
} // namespace

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

				//CloseHandle(process_info.hProcess);
				//CloseHandle(process_info.hThread);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to start process: {0}.", path);
			}
		}

		void run_process_within_window(std::string_view path)
		{
			const std::wstring wstr = std::wstring(path.begin(), path.end());

			STARTUPINFO startup_info = {sizeof(startup_info)};
			PROCESS_INFORMATION process_info;
			if (CreateProcess(wstr.c_str(), nullptr, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startup_info, &process_info))
			{
				WaitForInputIdle(process_info.hProcess, INFINITE);
				SetParent(FindWindowFromProcess(process_info.hProcess), glfwGetWin32Window(SL_HANDLE.window()->gl_window()));

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