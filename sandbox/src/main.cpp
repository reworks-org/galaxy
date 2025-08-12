///
/// main.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

// #include <iostream>

#include <mimalloc.h>
#include <mimalloc-new-delete.h>

// #include <galaxy/core/Application.hpp>
// #include <galaxy/core/Settings.hpp>
// #include <galaxy/scene/SceneManager.hpp>

using namespace galaxy;

int main(int argsc, char* argsv[])
{
	mi_version();

	// GALAXY_UNUSED(argsc);
	// GALAXY_UNUSED(argsv);

	// try
	// {
	// 	core::App sandbox("logs/", "config.json");

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
