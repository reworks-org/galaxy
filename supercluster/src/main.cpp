///
/// main.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/Application.hpp>
#include <galaxy/core/ServiceLocator.hpp>

#include "Editor.hpp"

class Editor : public galaxy::Application
{
public:
	Editor(std::unique_ptr<galaxy::Config>& config)
		:galaxy::Application(config)
	{
	}
};

int main(int argsc, char* argsv[])
{
	bool restart = false;

	do
	{
		restart = false;

		{
			std::unique_ptr<galaxy::Config> config = std::make_unique<galaxy::Config>();
			config->init("config.json");
			if (!config->open())
			{
				config->define<int>("threadpool-threadcount", 4);
				config->define<int>("anti-aliasing", 2);
				config->define<int>("ansio-filter", 2);
				config->define<bool>("vsync", false);
				config->define<bool>("srgb", false);
				config->define<int>("aspect-ratio-x", -1);
				config->define<int>("aspect-ratio-y", -1);
				config->define<bool>("raw-mouse-input", true);
				config->define<std::string>("window-name", "Sandbox");
				config->define<int>("window-width", 1280);
				config->define<int>("window-height", 720);
				config->define<bool>("is-cursor-visible", true);
				config->define<std::string>("cursor-image", "cursor.png");
				config->define<std::string>("icon-file", "icon.png");
				config->define<std::string>("root-path", "bin/");
				config->define<std::string>("textures-path", "textures/");
				config->define<std::string>("shaders-path", "shaders/");
				config->define<std::string>("scripts-path", "scripts/");
				config->define<std::string>("music-path", "music/");
				config->define<std::string>("sfx-path", "sfx/");

				config->create();
				config->open();
			}

			Editor editor(config);

			auto* gs = SL_HANDLE.gamestate();
			gs->create<sc::Editor>("Editor");
			gs->push("Editor");

			restart = editor.run();
		}

	} while (restart);

	return 0;
}