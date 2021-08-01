///
/// main.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "layers/Editor.hpp"
#include "layers/Game.hpp"
#include "layers/MainMenu.hpp"
#include "resources/Roboto-Light.hpp"

class Supercluster : public galaxy::core::Application
{
public:
	Supercluster(std::string_view asset_dir, std::string_view config_file)
		: Application {asset_dir, config_file}
	{
	}
};

int main(int argsc, char* argsv[])
{
	GALAXY_UNUSED(argsc);
	GALAXY_UNUSED(argsv);

	bool restart = false;

	do
	{
		restart                          = false;
		SL_HANDLE.m_restart              = false;
		SL_HANDLE.m_app_start_time_point = std::chrono::high_resolution_clock::now();

		if (!std::filesystem::exists("projects"))
		{
			std::filesystem::create_directory("projects");
		}

		{
			Supercluster app {"assets/", "assets/config.json"};
			SL_HANDLE.window()->prevent_native_closing();

			ImGuiIO& io    = ui::imgui_init_context();
			io.IniFilename = "assets/sclayout.ini";

			ImFontConfig font_config         = {};
			font_config.FontDataOwnedByAtlas = false;
			font_config.RasterizerMultiply   = 1.5f;
			font_config.OversampleH          = 4;
			io.FontDefault = io.Fonts->AddFontFromMemoryTTF(reinterpret_cast<void*>(&ttf::roboto_light), ttf::roboto_light_len, 16.0f, &font_config);

			auto editor    = app.create_layer<sc::Editor>();
			auto game      = app.create_layer<sc::Game>();
			auto main_menu = app.create_layer<sc::MainMenu>();

			main_menu->m_editor = editor;
			main_menu->m_app    = static_cast<core::Application*>(&app);

			app.push_layer(main_menu);
			restart = app.run();

			ui::imgui_destroy_context();
		}

	} while (restart);

	return EXIT_SUCCESS;
}