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

inline void setup_logging()
{
	// Configure logging.
	const auto        log_dir  = "logs/";
	const auto        now      = std::chrono::zoned_time {std::chrono::current_zone(), std::chrono::system_clock::now()}.get_local_time();
	const std::string log_path = std::format("{0}{1}{2}", log_dir, std::format("{0:%d-%m-%Y-[%H-%M]}", now), ".log");
	if (!std::filesystem::exists(log_dir))
	{
		std::filesystem::create_directory(log_dir);
	}

	entt::locator<galaxy::Log>::emplace();
	GALAXY_ADD_SINK(galaxy::FileSink, log_path);
	GALAXY_ADD_SINK(galaxy::ConsoleSink);
}

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
		GALAXY_UNUSED(e);
		GALAXY_LOG(GALAXY_ERROR, "CAUGHT FATAL EXCEPTION SUCCESSFULLY!");
	}
}

inline void logging_tests()
{
	setup_logging();
	print_all_levels();
}

#endif
