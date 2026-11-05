///
/// main.cpp
/// dist
///
/// Refer to LICENSE.txt for more details.
///

#define SDL_MAIN_HANDLED

#include <mimalloc.h>
#include <mimalloc-new-delete.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <tinyfiledialogs.h>

#include <galaxy/core/Application.hpp>
#include <galaxy/platform/Pragma.hpp>

int SDL_main(int argc, char* argv[])
{
	GALAXY_UNUSED(argc);
	GALAXY_UNUSED(argv);

	try
	{
		auto app = galaxy::App();
		app.load();
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
	mi_version();
	return SDL_RunApp(argc, argv, SDL_main, nullptr);
}
