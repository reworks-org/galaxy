///
/// main.cpp
/// dist
///
/// Refer to LICENSE.txt for more details.
///

#include <mimalloc-new-delete.h>
#include <tinyfiledialogs.h>

#include <galaxy/core/Application.hpp>
#include <galaxy/utils/Globals.hpp>

using namespace galaxy;

int main(int argsc, char* argsv[])
{
	GALAXY_UNUSED(argsc);
	GALAXY_UNUSED(argsv);

	try
	{
		core::App dist("logs/", "config.json");

		dist.load();
		dist.run();
	}
	catch (const std::exception& e)
	{
		tinyfd_notifyPopup("Error", e.what(), "error");
	}

	return GALAXY_EXIT_SUCCESS;
}
