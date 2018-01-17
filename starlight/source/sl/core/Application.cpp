///
///  Application.cpp
///  starlight
///
///  Created by reworks on 08/07/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#include "sl/fs/VFS.hpp"
#include "sl/core/World.hpp"
#include "sl/utils/Time.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/core/StateManager.hpp"
#include "sl/core/DebugInterface.hpp"
#include "sl/events/EventManager.hpp"
#include "sl/utils/ConfigReader.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/physics/Box2DManager.hpp"
#include "sl/resources/SoundPlayer.hpp"
#include "sl/resources/MusicPlayer.hpp"
#include "sl/graphics/TextureAtlas.hpp"
#include "sl/resources/ShaderLibrary.hpp"

#include "Application.hpp"

namespace sl
{
	Application::Application(const std::string& archive, const std::string& config, std::function<void(std::ofstream&)> newConfig)
	{
		std::srand(std::time(nullptr));

		std::string lname = "logs/log_" + Time::getFormattedTime() + ".log";
		loguru::add_file(lname.c_str(), loguru::Append, loguru::Verbosity_MAX);
		loguru::set_fatal_handler([](const loguru::Message& message)
		{
			al_show_native_message_box(NULL, "FATAL", message.prefix, message.message, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		});

		LOG_S(INFO) << "App init." << std::endl;

		al_install_system(ALLEGRO_VERSION_INT, atexit);
		al_install_keyboard();
		al_install_mouse();
		al_install_audio();

		al_init_font_addon();
		al_init_ttf_addon();
		al_init_image_addon();
		al_init_video_addon();
		al_init_acodec_addon();
		al_init_primitives_addon();
		al_init_native_dialog_addon();

		VFS::make(archive);
		ConfigReader::make(config, newConfig);

		Window::make(m_configReader->lookup<int>(config, "graphics", "width"), m_configReader->lookup<int>(config, "graphics", "height"), m_configReader->lookup<bool>(config, "graphics", "fullscreen"), m_configReader->lookup<bool>(config, "graphics", "msaa"), m_configReader->lookup<int>(config, "graphics", "msaaValue"), m_configReader->lookup<std::string>(config, "graphics", "title"), m_configReader->lookup<std::string>(config, "graphics", "icon"));

		World::make();
		DebugInterface::make(m_window->getDisplay());
		StateManager::make();
		TextureAtlas::make(m_configReader->lookup<size_t>(config, "graphics", "atlasPowerOf"));
		FontBook::make(m_configReader->lookup<std::string>(config, "fontmanager", "fontScript"));
		ShaderLibrary::make();
		MusicPlayer::make(m_configReader->lookup<std::string>(config, "audio", "musicScript"));
		SoundPlayer::make(m_configReader->lookup<std::string>(config, "audio", "soundScript"));
		Box2DManager::make(m_configReader->lookup<float32>(config, "box2d", "gravity"));
		EventManager::make();

		#ifdef NDEBUG
			DebugInterface::get()->disable(true);
		#endif

		al_reserve_samples(ConfigReader::get()->lookup<int>(config, "audio", "reserveSamples"));
	}

	Application::~Application()
	{
		EventManager::destroy();
		Box2DManager::destroy();
		SoundPlayer::destroy();
		MusicPlayer::destroy();
		ShaderLibrary::destroy();
		FontBook::destroy();
		TextureAtlas::destroy();
		StateManager::destroy();
		DebugInterface::destroy();
		World::destroy();
		Window::destroy();
		ConfigReader::destroy();
		VFS::destroy();

		al_shutdown_native_dialog_addon();
		al_shutdown_primitives_addon();
		al_shutdown_video_addon();
		al_shutdown_image_addon();
		al_shutdown_ttf_addon();
		al_shutdown_font_addon();

		al_uninstall_audio();
		al_uninstall_mouse();
		al_uninstall_keyboard();
		al_uninstall_system();
	}

	int Application::run()
	{
		#ifndef NDEBUG
			int frames = 0;
			int updates = 0;
		#endif

		double timePerFrame = 1.0 / 60.0;
		std::uint64_t timer = 0;

		World* world = World::get();
		Window* window = Window::get();
		StateManager* stateManager = StateManager::get();
		DebugInterface* debugInterface = DebugInterface::get();
		EventManager* eventManager = EventManager::get();

		ALLEGRO_TIMER* clock = al_create_timer(timePerFrame);
		
		al_register_event_source(eventManager->m_queue, al_get_display_event_source(window->getDisplay()));
		al_register_event_source(eventManager->m_queue, al_get_mouse_event_source());
		al_register_event_source(eventManager->m_queue, al_get_keyboard_event_source());
		al_register_event_source(eventManager->m_queue, al_get_timer_event_source(clock));

		timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		al_start_timer(clock);

		stateManager->load();

		while (window->isOpen())
		{
			ALLEGRO_EVENT ev;
			while (al_get_next_event(eventManager->, &ev))
			{
				stateManager->event(&ev);
				debugInterface->event(&ev);

				switch (ev.type)
				{
				case ALLEGRO_EVENT_TIMER:
					stateManager->update(timePerFrame);
					world->update(timePerFrame);

					#ifndef NDEBUG
						updates++;	
					#endif

					break;

				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					window->close();
					break;

				case ALLEGRO_EVENT_DISPLAY_RESIZE:
					ImGui_ImplA5_InvalidateDeviceObjects();
					al_acknowledge_resize(window->getDisplay());
					Imgui_ImplA5_CreateDeviceObjects();
					break;
				}
			}

			debugInterface->newFrame();
			debugInterface->displayMenu();

			window->clear(255, 255, 255);

			stateManager->render();
			debugInterface->render();

			window->display();

			#ifndef NDEBUG
				frames++;

				if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timer) > 1000)
				{
					timer += 1000;
					LOG_S(INFO) << updates << " ups, " << frames << " fps";

					updates = 0;
					frames = 0;
				}
			#endif
		}

		stateManager->unload();

		al_stop_timer(clock);
		al_destroy_timer(clock);

		LOG_S(INFO) << "App close.";

		return EXIT_SUCCESS;
	}
}