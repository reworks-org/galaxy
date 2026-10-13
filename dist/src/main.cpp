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
// #include <tinyfiledialogs.h>

// #include <galaxy/core/Application.hpp>
#include <galaxy/platform/Pragma.hpp>

// using namespace galaxy;

int SDL_main(int argc, char* argv[])
{
	mi_version();
	GALAXY_UNUSED(argc);
	GALAXY_UNUSED(argv);

	// try
	// {
	// 	core::App dist("logs/", "config.json");

	// 	dist.load();
	// 	dist.run();
	// }
	// catch (const std::exception& e)
	// {
	// 	tinyfd_notifyPopup("Error", e.what(), "error");
	// }

	return GALAXY_EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
	GALAXY_UNUSED(argc);
	GALAXY_UNUSED(argv);

	return SDL_RunApp(0, nullptr, SDL_main, nullptr);
}
