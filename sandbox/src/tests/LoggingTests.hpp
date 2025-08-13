///
/// LoggingTests.hpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOX_TESTS_LOGGINGTESTS_HPP_
#define SANDBOX_TESTS_LOGGINGTESTS_HPP_

#include <galaxy/logging/Log.hpp>

inline void print_all_levels()
{
	// Sinks are handled by the application.

	GALAXY_LOG(GALAXY_INFO, "INFO TEST.");
	GALAXY_LOG(GALAXY_WARNING, "WARNING TEST.");
	GALAXY_LOG(GALAXY_ERROR, "ERROR TEST.");

	try
	{
		GALAXY_LOG(GALAXY_FATAL, "FATAL TEST.");
	}
	catch (const std::exception& e)
	{
		// GALAXY_UNUSED(e);
		GALAXY_LOG(GALAXY_ERROR, "CAUGHT FATAL EXCEPTION SUCCESSFULLY!");
	}
}

#endif
