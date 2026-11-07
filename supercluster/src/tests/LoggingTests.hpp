///
/// LoggingTests.hpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOX_TESTS_LOGGINGTESTS_HPP_
#define SANDBOX_TESTS_LOGGINGTESTS_HPP_

#include <galaxy/logging/ConsoleSink.hpp>
#include <galaxy/logging/FileSink.hpp>
#include <galaxy/logging/Log.hpp>
#include <galaxy/platform/Pragma.hpp>

inline void print_all_levels()
{
	// Sinks are handled by the application.

	GALAXY_LOG(GALAXY_INFO, "INFO TEST.");
	GALAXY_LOG(GALAXY_WARN, "WARNING TEST.");
	GALAXY_LOG(GALAXY_ERROR, "ERROR TEST.");

	try
	{
		GALAXY_LOG(GALAXY_FATAL, "FATAL TEST.");
	}
	catch (const std::runtime_error& e)
	{
		GALAXY_UNUSED(e);
		GALAXY_LOG(GALAXY_ERROR, "CAUGHT FATAL EXCEPTION SUCCESSFULLY!");
	}
}

inline void logging_tests()
{
	print_all_levels();
}

#endif
