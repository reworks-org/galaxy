///
/// main.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#include <portable-file-dialogs.h>

#include <galaxy/core/Application.hpp>
#include <galaxy/core/ServiceLocator.hpp>

#include <galaxy/systems/RenderSystem.hpp>

#include "states/Editor.hpp"

class EditorApp : public galaxy::Application
{
public:
	EditorApp(std::unique_ptr<galaxy::Config>& config)
	    : galaxy::Application(config)
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

		try
		{
			// Load DateTime db.
			date::set_install("assets/tz/tzdata");
			date::reload_tzdb();

			// Logging.
			std::string log_path = fmt::format("{0}{1}", date::format("%m/%d/%Y %H:%M\n", date::make_zoned(date::current_zone(), std::chrono::system_clock::now())), ".log");
			PL_LOG_START(log_path);
			PL_LOG_GET.set_min_level(PL_INFO);

			auto config = std::make_unique<galaxy::Config>();
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
				if (!config->open())
				{
					throw std::runtime_error("Failed to create config file a second time.");
				}
			}

			EditorApp editor(config);

			auto* gs = SL_HANDLE.gamestate();
			gs->create<sc::Editor>("Editor");
			gs->push("Editor");

			auto* world = SL_HANDLE.world();
			world->create_system<galaxy::RenderSystem>();

			restart = editor.run();
		}
		catch (std::exception& e)
		{
			pfd::message("FATAL EXCEPTION", e.what(), pfd::choice::ok, pfd::icon::error);
		}
		catch (...)
		{
			pfd::message("UNKNOWN EXCEPTION", "Did not utilize std::exception, invalid exception thrown.", pfd::choice::ok, pfd::icon::error);
		}

	} while (restart);

	return 0;
}