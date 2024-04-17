///
/// main.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <mimalloc-new-delete.h>

#include <imgui/imgui_freetype.h>
#include <imgui/imnotify/material_design_icons.h>
#include <imgui/imnotify/material_design_icons_ttf.h>

#include <galaxy/core/Application.hpp>
#include <galaxy/core/Config.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/resource/embedded/RobotoLight.hpp>
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
				core::ServiceLocator<core::Window>::ref().set_icon(data);

				if (!std::filesystem::exists(GALAXY_ROOT_DIR / GALAXY_EDITOR_DATA_DIR / "projects"))
				{
					std::filesystem::create_directory(GALAXY_ROOT_DIR / GALAXY_EDITOR_DATA_DIR / "projects");
				}

				ImGuiIO& io    = ui::imgui_init_context();
				io.IniFilename = "layout.ini";

				ImFontConfig font_cfg          = {};
				font_cfg.FontDataOwnedByAtlas  = false;
				font_cfg.RasterizerMultiply    = 1.5f;
				font_cfg.OversampleH           = 1;
				font_cfg.OversampleV           = 1;
				font_cfg.FontBuilderFlags     |= ImGuiFreeTypeBuilderFlags_LoadColor;
				io.FontDefault = io.Fonts->AddFontFromMemoryTTF(reinterpret_cast<void*>(&resource::roboto_light), resource::roboto_light_len, 16.0f, &font_cfg);

				static const ImWchar icons_ranges[] = {ICON_MIN_MDI, ICON_MAX_MDI, 0};
				ImFontConfig         md_icons_cfg;
				md_icons_cfg.FontDataOwnedByAtlas = false;
				md_icons_cfg.MergeMode            = true;
				md_icons_cfg.PixelSnapH           = true;
				ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)materialdesignicons_ttf, sizeof(materialdesignicons_ttf), 16.0f, &md_icons_cfg, icons_ranges);

				ui::imgui_set_theme();

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
