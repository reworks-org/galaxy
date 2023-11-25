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
#include "scenes/Menu.hpp"

using namespace galaxy;

class Supercluster : public core::Application
{
  public:
	Supercluster()
		: Application {"logs/", "config.json"}
	{
	}

	virtual ~Supercluster() = default;
};

int main(int argsc, char* argsv[])
{
	GALAXY_UNUSED(argsc);
	GALAXY_UNUSED(argsv);

	do
	{
		GALAXY_RESTART = false;

#ifdef NDEBUG
		try
		{
#endif
			Supercluster app;

			{
				auto data = core::ServiceLocator<fs::VirtualFileSystem>::ref().read_disk<meta::FSBinaryR>("editor/sc.png");
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
				ImGui_Notify::MergeIconsWithLatestFont(16.0f, false);

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

				auto& sm = core::ServiceLocator<scene::SceneManager>::ref();

				auto menu   = std::make_shared<sc::Menu>("sc_menu");
				auto editor = std::make_shared<sc::Editor>("sc_editor");

				sm.add_existing_scene(menu->m_name, menu);
				sm.add_existing_scene(editor->m_name, editor);
				sm.set_scene("sc_menu");
			}

			app.run();
			ui::imgui_destroy_context();

#ifdef NDEBUG
		}
		catch (const std::exception& e)
		{
			std::cout << "======================" << std::endl;
			std::cout << " UNHANDLED EXCEPTION: " << e.what() << std::endl;
			std::cout << "======================" << std::endl;
		}
#endif
	} while (GALAXY_RESTART);

	return GALAXY_EXIT_SUCCESS;
}
