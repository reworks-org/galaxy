///
/// main.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <imgui_addons/imgui_notify.h>
#include <imgui_freetype.h>
#include <mimalloc-new-delete.h>

#include <galaxy/core/Application.hpp>
#include <galaxy/core/Config.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/embedded/RobotoLight.hpp>
#include <galaxy/scene/SceneManager.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>
#include <galaxy/ui/ImGuiTheme.hpp>
#include <galaxy/utils/Globals.hpp>

#include "scenes/Editor.hpp"

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
				io.FontDefault = io.Fonts->AddFontFromMemoryTTF(reinterpret_cast<void*>(&embedded::roboto_light), embedded::roboto_light_len, 16.0f, &font_cfg);
				ImGui::MergeIconsWithLatestFont(16.0f, false);

				auto& config = core::ServiceLocator<core::Config>::ref();
				config.restore<std::string>("theme", "DARK", "editor");
				config.save();

				const auto theme = magic_enum::enum_cast<ui::ImguiThemeId>(config.get<std::string>("theme", "editor"));
				if (theme.has_value())
				{
					if (theme.value() == ui::ImguiThemeId::DARK)
					{
						ImGui::StyleColorsDark();
					}
					else if (theme.value() == ui::ImguiThemeId::DRACULA)
					{
						ui::imgui_theme_dracula();
					}
					else if (theme.value() == ui::ImguiThemeId::MATERIAL_DARK)
					{
						ui::imgui_theme_material_dark();
					}
					else if (theme.value() == ui::ImguiThemeId::VISUAL_DARK)
					{
						ui::imgui_theme_visual_dark();
					}
					else if (theme.value() == ui::ImguiThemeId::FANCY_DARK)
					{
						ui::imgui_theme_fancy_dark();
					}
					else if (theme.value() == ui::ImguiThemeId::DARK_EMBRACE)
					{
						ui::imgui_theme_dark_embrace();
					}
					else if (theme.value() == ui::ImguiThemeId::ENHANCED_DARK)
					{
						ui::imgui_theme_enhanced_dark();
					}
				}

				auto& sm    = core::ServiceLocator<scene::SceneManager>::ref();
				auto  scene = sm.add_custom<sc::Editor>("sc_editor");
				scene->enable();
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
