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
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#include "loguru/loguru.hpp"
#include "re/utils/Time.hpp"
#include "re/services/ServiceLocator.hpp"

#include "Application.hpp"

namespace re
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
		re::Locator::provide<re::VFS>(m_vfs);
		
		m_engineConfig = new ConfigReader(config, newConfig);
		
		m_window = new Window(m_engineConfig->lookup<int>("graphics", "width"), m_engineConfig->lookup<int>("graphics", "height"), m_engineConfig->lookup<bool>("graphics", "fullscreen"), m_engineConfig->lookup<bool>("graphics", "msaa"), m_engineConfig->lookup<int>("graphics", "msaaValue"), m_engineConfig->lookup<std::string>("graphics", "title"), m_engineConfig->lookup<std::string>("graphics", "icon"));
		re::Locator::provide<re::Window>(m_window);
		
		m_world = new World();
		re::Locator::provide<re::World>(m_world);
		
		m_stateManager = new StateManager();
		re::Locator::provide<re::StateManager>(m_stateManager);
		
		m_fontManager = new FontManager(m_engineConfig->lookup<std::string>("fontmanager", "fontScript"));
		re::Locator::provide<re::FontManager>(m_fontManager);
		
		m_audioManager = new AudioManager(m_engineConfig->lookup<std::string>("audiomanager", "audioScript"), m_engineConfig->lookup<int>("audiomanager", "reserveSamples"));
		re::Locator::provide<re::AudioManager>(m_audioManager);
		
		m_b2dManager = new Box2DManager(m_engineConfig->lookup<float32>("box2d", "gravity"));
		re::Locator::provide<re::Box2DManager>(m_b2dManager);
		
		m_debugManager = new DebugManager(m_window->getDisplay());
		re::Locator::provide<re::DebugManager>(m_debugManager);
		
		m_textureAtlas = new TextureAtlas(m_engineConfig->lookup<std::string>("graphics", "atlas"));
		re::Locator::provide<re::TextureAtlas>(m_textureAtlas);
		
		// TEMP
		m_camera = new Camera({ 0, 0, m_engineConfig->lookup<int>("graphics", "width"), m_engineConfig->lookup<int>("graphics", "height") });
		re::Locator::provide<re::Camera>(m_camera);

		#ifdef NDEBUG
			m_debugManager->disable(true);
		#endif
	}

	Application::~Application()
	{
		delete m_camera;
		delete m_textureAtlas;
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
		ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();

		al_register_event_source(eventQueue, al_get_display_event_source(m_window->getDisplay()));
		al_register_event_source(eventQueue, al_get_mouse_event_source());
		al_register_event_source(eventQueue, al_get_keyboard_event_source());
		al_register_event_source(eventQueue, al_get_timer_event_source(clock));

		timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		al_start_timer(clock);

		m_stateManager->load();

		while (m_window->isOpen())
		{
			ALLEGRO_EVENT ev;
			while (al_get_next_event(eventQueue, &ev))
			{
				m_stateManager->event(&ev);
				m_debugManager->event(&ev);

				switch (ev.type)
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

			m_debugManager->newFrame();
			m_debugManager->displayMenu();

			m_window->clear(255, 255, 255);

			m_stateManager->render();
			m_debugManager->render();

			m_window->display();
			frames++;

			if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timer) > 1000)
			{
				timer += 1000;
				LOG_S(INFO) << m_appTitle << "  |  " + updates << " ups, " << frames << " fps" << std::endl;
				
				updates = 0;
				frames = 0;
			}
		}

		m_stateManager->unload();

		al_stop_timer(clock);
		al_destroy_event_queue(eventQueue);
		al_destroy_timer(clock);
		LOG_S(INFO) << "App close." << std::endl;

		return EXIT_SUCCESS;
	}
}