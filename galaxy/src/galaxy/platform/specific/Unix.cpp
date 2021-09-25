///
/// Unix.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include "Unix.hpp"

#ifdef GALAXY_UNIX_PLATFORM

namespace galaxy
{
	namespace platform
	{
		void configure_terminal()
		{
			GALAXY_LOG(GALAXY_FATAL, "NYI.");
		}

		void* run_process(std::string_view path)
		{
			GALAXY_LOG(GALAXY_FATAL, "NYI.");
		}

		void close_process(void* process)
		{
			GALAXY_LOG(GALAXY_FATAL, "NYI.");
		}
	} // namespace platform
} // namespace galaxy

#endif