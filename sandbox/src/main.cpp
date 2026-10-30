///
/// main.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#define SDL_MAIN_HANDLED

#include <print>
#include <mimalloc.h>
#include <mimalloc-new-delete.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <tinyfiledialogs.h>

#include "tests/LoggingTests.hpp"
#include "tests/PlatformTests.hpp"

// #include "Sandbox.hpp"
#include "TestConfig.hpp"

#include <galaxy/core/Application.hpp>
#include <galaxy/platform/Pragma.hpp>

int SDL_main(int argc, char* argv[])
{
	mi_version();
	GALAXY_UNUSED(argc);
	GALAXY_UNUSED(argv);

	try
	{
		auto app = galaxy::App();

		if (ENABLE_PLATFORM_TESTS)
		{
			platform_tests();
		}

		if (ENABLE_LOGGING_TESTS)
		{
			logging_tests();
		}

		// if (ENABLE_SANDBOX)
		//{
		//	auto& sm = entt::locator<galaxy::SceneManager>::value();
		//	sm.add<Sandbox>("Sandbox");
		//	sm.push("Sandbox");
		// }

		app.run();
	}
	catch (const std::exception& e)
	{
		const std::string msg = {e.what()};
		tinyfd_messageBox("Error", msg.c_str(), "ok", "error", 1);
	}

	return GALAXY_EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
	GALAXY_UNUSED(argc);
	GALAXY_UNUSED(argv);

	return SDL_RunApp(0, nullptr, SDL_main, nullptr);
}
