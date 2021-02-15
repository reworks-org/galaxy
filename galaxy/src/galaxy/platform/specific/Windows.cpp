///
/// Windows.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifdef _WIN32 || _WIN64

#include <filesystem>

#include <Windows.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
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

		void* run_process(std::string_view path)
		{
			const auto fs_path = std::filesystem::path(path).string();
			const auto wstr    = std::wstring(fs_path.begin(), fs_path.end());

			STARTUPINFO startup_info = {sizeof(startup_info)};
			PROCESS_INFORMATION process_info;
			if (CreateProcess(wstr.c_str(), nullptr, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startup_info, &process_info))
			{
				WaitForInputIdle(process_info.hProcess, INFINITE);
				CloseHandle(process_info.hThread);

				return reinterpret_cast<void*>(process_info.hProcess);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to start process: {0}.", path);
				return nullptr;
			}
		}

		void close_process(void* process)
		{
			if (process != nullptr)
			{
				TerminateProcess(process, 0);
				CloseHandle(process);
			}
		}
	} // namespace platform
} // namespace galaxy

#endif