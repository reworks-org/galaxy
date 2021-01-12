///
/// Linux.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef _WIN32 || _WIN64

#include "Linux.hpp"

namespace galaxy
{
	namespace platform
	{
		void configure_terminal()
		{
		}

		void run_process(std::string_view path)
		{
		}
	} // namespace platform
} // namespace galaxy

#endif