//
//  Application.cpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <chrono>

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_native_dialog.h>

#include "re/utility/Log.hpp"

#include "Application.hpp"

void engineConfig(std::ofstream& newFile)
{
	newFile << "appTitle = \"Default\"" << std::endl;
	newFile << "assetPath = \"bin/assets/\"" << std::endl;
	newFile << "ups = 60.0" << std::endl;
	newFile << "versionMajor = 1" << std::endl;
	newFile << "versionMinor = 0" << std::endl;
	newFile << "versionPatch = 0" << std::endl;
	newFile << "screenWidth = 640" << std::endl;
	newFile << "screenHeight = 480" << std::endl;
	newFile << "renderingLayers = 2" << std::endl;
	newFile << "framerateLimit = 0" << std::endl;
	newFile << "keyRepeat = true" << std::endl;
	newFile << "cursorVisible = true" << std::endl;
	newFile << "vsyncEnabled = false" << std::endl;
	newFile << "saveLog = false" << std::endl;
	newFile << "enableDebug = false" << std::endl;
}

namespace re
{
	Application::Application(float32 gravity)
	:m_window(),
	 m_physicsManager(gravity),
	 m_engineConfig("config.cfg", engineConfig)
	{
		RE_LOG_PROVIDEWINDOW(&window);
		RE_LOG_PRINTPRETTY(LogLevel::WARNING, "REngine3 Initialization Begin");

		al_init();
		al_init_font_addon();
		al_init_image_addon();
		al_init_native_dialog_addon();

		al_install_keyboard();
		al_install_mouse();

		m_world.init();
	}

	Application::~Application()
	{
		al_uninstall_mouse();
		al_uninstall_keyboard();

		al_shutdown_native_dialog_addon();
		al_shutdown_image_addon();
		al_shutdown_font_addon();

		al_uninstall_system();
	}

	int Application::run()
	{
		std::uint64_t timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		int frames = 0;
		int updates = 0;

		ALLEGRO_TIMER* l_timer = al_create_timer(1.0 / m_ups);
		ALLEGRO_TIMER* l_dt = al_create_timer(1.0 / m_ups);
		std::uint64_t l_timeSinceLastUpdate = 0;
		double l_timePerFrame = 1.0 / m_ups;
		ALLEGRO_EVENT l_event;

		m_stateManager.load();
		m_debugManager.init(m_window, m_enableDebug);

		al_start_timer(l_timer);
		while (m_window.isOpen())
		{
			al_set_timer_count(l_dt, al_get_timer_count(l_timer));
			al_set_timer_count(l_timer, 0);
			l_timeSinceLastUpdate += al_get_timer_count(l_dt);
            
			while (l_timeSinceLastUpdate > l_timePerFrame)
			{
				l_timeSinceLastUpdate -= l_timePerFrame;
				
                // Handle events that need to be polled
                while (al_get_next_event(m_window.getEvents(), &l_event))
                {
                    switch(l_event.type)
                    {
					case ALLEGRO_EVENT_DISPLAY_CLOSE:
						m_window.close();
						break;
                    }
                    
                    m_debugManager.event(&l_event);
                    m_stateManager.handlePollEvents(&l_event);
                }
                
                m_stateManager.handleEvents(m_window.m_event);
                
				// Update
				m_debugManager.update(m_window, TimePerFrame);
				m_stateManager.update(TimePerFrame);

                // Display the debug manager
                m_debugManager.useMenu();
                
				updates++;
            }
            
			// Render
			m_window.clear(sf::Color::Black);

			m_stateManager.render();
			m_debugManager.render(m_window);

			m_window.display();
			frames++;
			
			if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timer) > 1000)
			{
				timer += 1000;

				std::string header = m_appTitle + "  |  " + std::to_string(updates) + " ups, " + std::to_string(frames) + " fps";

                RE_LOG(LogLevel::INFO, header, "Application::run", "Application.cpp", 69);
				m_window.setTitle(header);

				updates = 0;
				frames = 0;
			}
		}
		al_stop_timer(l_timer);
		al_destroy_timer(l_dt);
		al_destroy_timer(l_timer);

		m_stateManager.unload();

		RE_LOG(LogLevel::INFO, "Program quit successfully", "Application::run", "Application.cpp", 78);
		RE_LOG_SAVE(m_saveLog);

		return EXIT_SUCCESS;
	}
}
