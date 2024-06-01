///
/// main.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <mimalloc-new-delete.h>

#include <galaxy/core/Application.hpp>
#include <galaxy/core/Config.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "Editor.hpp"

using namespace galaxy;

int main(int argsc, char* argsv[])
{
	GALAXY_UNUSED(argsc);
	GALAXY_UNUSED(argsv);

	do
	{
		GALAXY_RESTART = false;

		try
		{
			core::App supercluster("logs/", "config.json");

			{
				auto data = core::ServiceLocator<fs::VirtualFileSystem>::ref().read_binary("sc.png");

				auto& window = core::ServiceLocator<core::Window>::ref();
				window.set_icon(data);

				if (!std::filesystem::exists(GALAXY_ROOT_DIR / GALAXY_EDITOR_DATA_DIR / "projects"))
				{
					std::filesystem::create_directory(GALAXY_ROOT_DIR / GALAXY_EDITOR_DATA_DIR / "projects");
				}

				ui::imgui_init_context("layout.ini");

				auto& sm = core::ServiceLocator<scene::SceneManager>::ref();
				sm.add_custom<sc::Editor>("sc_editor");
				sm.set_scene("sc_editor");
			}

			supercluster.run();
			ui::imgui_destroy_context();
		}
		catch (const std::exception& e)
		{
			std::cout << "======================" << std::endl;
			std::cout << " UNHANDLED EXCEPTION: " << e.what() << std::endl;
			std::cout << "======================" << std::endl;
			std::cin.get();
		}
	} while (GALAXY_RESTART);

	return GALAXY_EXIT_SUCCESS;
}
