///
/// main.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#include <mimalloc.h>
#include <mimalloc-new-delete.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "tests/LoggingTests.hpp"
#include "tests/PlatformTests.hpp"

#include "Sandbox.hpp"
#include "TestConfig.hpp"

int main(int argc, char* argv[])
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

		if (ENABLE_SANDBOX)
		{
			auto& sm = entt::locator<galaxy::SceneManager>::value();
			sm.add<Sandbox>("Sandbox");
			sm.push("Sandbox");
		}

		app.run();
	}
	catch (const std::exception& e)
	{
		tinyfd_notifyPopup("Error", e.what(), "error");
	}

	return 0;
}
