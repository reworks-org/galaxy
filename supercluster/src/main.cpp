///
/// main.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/Application.hpp>
#include <galaxy/core/ServiceLocator.hpp>

#include <galaxy/systems/RenderSystem.hpp>

#include "states/Editor.hpp"

class EditorApp : public galaxy::Application
{
public:
	EditorApp()
	    : galaxy::Application()
	{
	}
};

int main(int argsc, char* argsv[])
{
	bool restart = false;

	do
	{
		restart             = false;
		SL_HANDLE.m_restart = false;

		{
			EditorApp editor;
			auto* config = editor.make_config();
			config->init("assets/config.json");
			if (!config->open())
			{
				config->define<int>("threadpool-threadcount", 4);
				config->define<int>("anti-aliasing", 2);
				config->define<int>("ansio-filter", 2);
				config->define<bool>("vsync", true);
				config->define<bool>("srgb", false);
				config->define<int>("aspect-ratio-x", -1);
				config->define<int>("aspect-ratio-y", -1);
				config->define<bool>("raw-mouse-input", true);
				config->define<std::string>("window-name", "Supercluster - Galaxy Editor");
				config->define<int>("window-width", 1280);
				config->define<int>("window-height", 720);
				config->define<bool>("is-cursor-visible", true);
				config->define<std::string>("cursor-image", "cursor.png");
				config->define<std::string>("icon-file", "icon.png");
				config->define<std::string>("root-path", "assets/");
				config->define<std::string>("textures-path", "textures/");
				config->define<std::string>("shaders-path", "shaders/");
				config->define<std::string>("scripts-path", "scripts/");
				config->define<std::string>("audio-path", "audio/");
				config->define<std::string>("json-path", "json/");
				config->define<std::string>("font-path", "fonts/");
				config->define<std::string>("save-folder", "saves/");
				config->define<std::string>("fontbook-json", "fontbook.json");
				config->define<std::string>("shaderbook-json", "shaderbook.json");
				config->define<std::string>("audiobook-json", "audiobook.json");

				config->create();
				config->open();
			}

			auto* gs = SL_HANDLE.gamestate();
			gs->create<sc::Editor>("Editor");
			gs->push("Editor");

			auto* world = SL_HANDLE.world();
			world->create_system<galaxy::RenderSystem>();

			restart = editor.run();
		}

	} while (restart);

	return 0;
}