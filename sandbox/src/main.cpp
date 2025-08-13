///
/// main.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

// #include <iostream>
#include <format>

#include <mimalloc.h>
#include <mimalloc-new-delete.h>

// #include <galaxy/core/Application.hpp>
// #include <galaxy/core/Settings.hpp>
// #include <galaxy/scene/SceneManager.hpp>

#include <galaxy/logging/ConsoleSink.hpp>
#include <galaxy/logging/FileSink.hpp>
#include <galaxy/platform/Platform.hpp>

#include "tests/LoggingTests.hpp"

int main(int argsc, char* argsv[])
{
	mi_version();

	// GALAXY_UNUSED(argsc);
	// GALAXY_UNUSED(argsv);

	// try
	// {
	// 	core::App sandbox("logs/", "config.json");

	galaxy::platform::configure_terminal();

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

	print_all_levels();

	// 	{
	// 		auto& sm = entt::locator<scene::SceneManager>::value();
	// 		sm.create("sandbox");
	// 		sm.set("sandbox");
	// 	}

	// 	sandbox.run();
	// }
	// catch (const std::exception& e)
	// {
	// 	std::cout << "======================" << std::endl;
	// 	std::cout << " UNHANDLED EXCEPTION: " << e.what() << std::endl;
	// 	std::cout << "======================" << std::endl;
	// 	std::cin.get();
	// }

	return 0;
}
