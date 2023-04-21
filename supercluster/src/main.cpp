///
/// main.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <mimalloc-new-delete.h>
#include <imgui_freetype.h>
#include <imgui_addons/imgui_notify.h>

#include <galaxy/core/Application.hpp>
#include <galaxy/core/Config.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/embedded/RobotoLight.hpp>
#include <galaxy/scene/SceneManager.hpp>
#include <galaxy/ui/ImGuiTheme.hpp>
#include <galaxy/utils/Globals.hpp>

#include "layers/Editor.hpp"
#include "layers/Menu.hpp"

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
				auto& window = core::ServiceLocator<core::Window>::ref();
				window.prevent_native_closing();

				auto& config = core::ServiceLocator<core::Config>::ref();
				auto root    = config.get<std::string>("asset_dir");

				if (!std::filesystem::exists(root + "../editor_data"))
				{
					std::filesystem::create_directory(root + "../editor_data");
				}

				if (!std::filesystem::exists(root + "../editor_data/projects"))
				{
					std::filesystem::create_directory(root + "../editor_data/projects");
				}

				ImGuiIO& io    = ui::imgui_init_context();
				io.IniFilename = "sclayout.ini";

				ImFontConfig font_cfg         = {};
				font_cfg.FontDataOwnedByAtlas = false;
				font_cfg.RasterizerMultiply   = 1.5f;
				font_cfg.OversampleH          = 4;
				font_cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;
				io.FontDefault = io.Fonts->AddFontFromMemoryTTF(reinterpret_cast<void*>(&embedded::roboto_light), embedded::roboto_light_len, 16.0f, &font_cfg);
				ImGui_Notify::MergeIconsWithLatestFont(16.f, false);

				config.restore<std::string>("theme", "CLASSIC", "editor");
				config.save();

				const auto theme = magic_enum::enum_cast<ui::ImguiThemeId>(config.get<std::string>("theme", "editor"));
				if (theme.has_value())
				{
					if (theme.value() == ui::ImguiThemeId::LIGHT)
					{
						ImGui::StyleColorsLight();
					}
					else if (theme.value() == ui::ImguiThemeId::DARK)
					{
						ImGui::StyleColorsDark();
					}
					else if (theme.value() == ui::ImguiThemeId::CLASSIC)
					{
						ImGui::StyleColorsClassic();
					}
					else if (theme.value() == ui::ImguiThemeId::ENHANCED_LIGHT)
					{
						ui::imgui_theme_enhanced_light();
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
				}

				auto& sm = core::ServiceLocator<scene::SceneManager>::ref();
				sm.load_assets();

				auto scene = sm.make("supercluster");
				if (auto ptr = scene.lock())
				{
					sm.make("game_runtime");

					sm.set(ptr->get_name());

					scene::LayerRegistry::register_type<sc::Editor>("Editor");
					scene::LayerRegistry::register_type<sc::Menu>("Menu");

					auto& layers = ptr->layers();

					layers.make<sc::Editor>("Editor");
					layers.make<sc::Menu>("Menu");

					layers.push("Menu");
				}
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