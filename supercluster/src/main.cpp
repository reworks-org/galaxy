///
/// main.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <galaxy/core/Application.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/state/SceneManager.hpp>
#include <galaxy/ui/ImGuiTheme.hpp>
#include <galaxy/utils/Globals.hpp>

#include "layers/Editor.hpp"
#include "layers/Menu.hpp"
#include "resources/RobotoLight.hpp"

using namespace galaxy;

class Supercluster : public core::Application
{
public:
	Supercluster()
		: Application {"logs/", "assets/editor_data/config.json"}
	{
	}

	virtual ~Supercluster() noexcept = default;
};

int main(int argsc, char* argsv[])
{
	GALAXY_UNUSED(argsc);
	GALAXY_UNUSED(argsv);

	do
	{
		GALAXY_RESTART  = false;
		GALAXY_START_TP = std::chrono::high_resolution_clock::now();

		if (!std::filesystem::exists("assets/editor_data"))
		{
			std::filesystem::create_directory("assets/editor_data");
		}

		if (!std::filesystem::exists("assets/editor_data/projects"))
		{
			std::filesystem::create_directory("assets/editor_data/projects");
		}

		try
		{
			Supercluster app;

			{
				auto& window = core::ServiceLocator<core::Window>::ref();
				window.prevent_native_closing();

				ImGuiIO& io    = ui::imgui_init_context();
				io.IniFilename = "assets/editor_data/sclayout.ini";

				ImFontConfig font_config         = {};
				font_config.FontDataOwnedByAtlas = false;
				font_config.RasterizerMultiply   = 1.5f;
				font_config.OversampleH          = 4;
				io.FontDefault = io.Fonts->AddFontFromMemoryTTF(reinterpret_cast<void*>(&ttf::roboto_light), ttf::roboto_light_len, 16.0f, &font_config);

				ui::imgui_theme_visual_dark();

				auto& sm   = core::ServiceLocator<state::SceneManager>::ref();
				auto scene = sm.make("supercluster");
				if (auto ptr = scene.lock())
				{
					sm.make("game_runtime");

					sm.set(ptr->get_name());

					state::LayerRegistry::register_type<sc::Editor>("Editor");
					state::LayerRegistry::register_type<sc::Menu>("Menu");

					auto& layers = ptr->layers();

					layers.make<sc::Editor>("Editor");
					layers.make<sc::Menu>("Menu");

					layers.push("Menu");
				}
			}

			app.run();
			ui::imgui_destroy_context();
		}
		catch (const std::exception& e)
		{
			std::cout << "======================" << std::endl;
			std::cout << " UNHANDLED EXCEPTION: " << e.what() << std::endl;
			std::cout << "======================" << std::endl;

#ifdef _DEBUG
			std::getchar();
#endif
		}
	} while (GALAXY_RESTART);

	return GALAXY_EXIT_SUCCESS;
}