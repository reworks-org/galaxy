//
//  Application.cpp
//  rework
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <allegro5/events.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#include "re/utility/Log.hpp"
#include "re/utility/Time.hpp"

#include "Application.hpp"

namespace re
{
	Application::Application(const std::string& archive, const std::string& config, std::function<void(std::ofstream&)>& newConfig)
	{
		Log::init();
		BOOST_LOG_TRIVIAL(info) << "App init." << std::endl;

		std::srand(std::time(nullptr));

		al_install_system(ALLEGRO_VERSION_INT, atexit);
		al_install_audio();
		al_install_mouse();
		al_install_keyboard();

		al_init_font_addon();
		al_init_ttf_addon();
		al_init_image_addon();
		al_init_video_addon();
		al_init_acodec_addon();
		al_init_primitives_addon();
		al_init_native_dialog_addon();

		m_vfs = new VFS(archive);
		m_engineConfig = new ConfigReader(config, newConfig);
		m_window = new Window(m_engineConfig->lookup<int>("window", "width"), m_engineConfig->lookup<int>("window", "height"), m_engineConfig->lookup<bool>("window", "fullscreen"), m_engineConfig->lookup<bool>("window", "msaa"), m_engineConfig->lookup<int>("window", "msaaValue"), m_engineConfig->lookup<std::string>("window", "title"), m_engineConfig->lookup<std::string>("window", "icon"));
		m_world = new World();
		m_stateManager = new StateManager();
		m_fontManager = new FontManager();
		m_b2dManager = new Box2DManager(m_engineConfig->lookup<float32>("box2d", "gravity"));
		m_debugManager = new DebugManager(m_window->getDisplay());
	}

	Application::~Application()
	{
		delete m_debugManager;
		delete m_b2dManager;
		delete m_fontManager;
		delete m_stateManager;
		delete m_world;
		delete m_window;
		delete m_engineConfig;
		delete m_vfs;

		al_shutdown_native_dialog_addon();
		al_shutdown_primitives_addon();
		al_shutdown_video_addon();
		al_shutdown_image_addon();
		al_shutdown_ttf_addon();
		al_shutdown_font_addon();

		al_uninstall_keyboard();
		al_uninstall_mouse();
		al_uninstall_audio();
		al_uninstall_system();
	}

	int Application::run()
	{
		int frames = 0;
		int updates = 0;
		double timePerFrame = 1.0 / 60.0;
		std::uint64_t timer = 0;

		ALLEGRO_TIMER* clock = al_create_timer(timePerFrame);
		ALLEGRO_EVENT event;
		ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();

		al_register_event_source(eventQueue, al_get_display_event_source(m_window->getDisplay()));
		al_register_event_source(eventQueue, al_get_mouse_event_source());
		al_register_event_source(eventQueue, al_get_keyboard_event_source());
		al_register_event_source(eventQueue, al_get_timer_event_source(clock));

		timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		al_start_timer(clock);

		m_stateManager->load();

		while (m_window->isOpen());
		{
			while (al_get_next_event(eventQueue, &event))
			{
				m_stateManager->event(&event);

				switch (event.type)
				{
				case ALLEGRO_EVENT_TIMER:
					m_stateManager->update(timePerFrame);
					m_world->update(timePerFrame);
					updates++;	
					break;

				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					m_window->close();
					break;
				}
			}

			m_window->clear(255, 255, 255);

			m_stateManager->render();

			m_window->display();
			frames++;

			if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timer) > 1000)
			{
				timer += 1000;
				BOOST_LOG_TRIVIAL(info) << m_appTitle << "  |  " + updates << " ups, " << frames << " fps" << std::endl;
				
				updates = 0;
				frames = 0;
			}
		}

		m_stateManager->unload();

		al_stop_timer(clock);
		al_destroy_event_queue(eventQueue);
		al_destroy_timer(clock);
		BOOST_LOG_TRIVIAL(info) << "App close." << std::endl;

		return EXIT_SUCCESS;
	}
}