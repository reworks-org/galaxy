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
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#include "re/utility/Log.hpp"

#include "Application.hpp"

namespace re
{
	Application::Application(int vMajor, int vMinor, int vPatch, double ups, bool saveLog, int width, int height, bool fullscreen, int msaa, int msaaValue, const std::string& title, const std::string& icon, float32 gravity)
	:m_versionMajor(vMajor),
	 m_versionMinor(vMinor),
	 m_versionPatch(vPatch),
	 m_ups(ups),
	 m_saveLog(saveLog),
	 m_appTitle(title),
	 m_vfs(), 
	 m_window(width, height, fullscreen, msaa, msaaValue, title, icon), 
	 m_world(), 
	 m_stateManager(), 
	 m_fontManager(), 
	 m_b2dManager(gravity), 
	 m_debugManager(m_window.getDisplay())
	{
		RE_LOG_PROVIDEWINDOW(&m_window);

		al_init();
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
	}

	Application::~Application()
	{
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
		std::uint64_t timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		int frames = 0;
		int updates = 0;

		ALLEGRO_TIMER* l_timer = al_create_timer(1.0 / m_ups);
		ALLEGRO_TIMER* l_dt = al_create_timer(1.0 / m_ups);
		std::uint64_t l_timeSinceLastUpdate = 0;
		double l_timePerFrame = 1.0 / m_ups;
		ALLEGRO_EVENT l_event;

		m_stateManager.load();

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
                }
                
                m_stateManager.event(&l_event);
                
				// Update
				m_stateManager.update(l_dt);
				m_debugManager.update();
                
				updates++;
            }
            
			// Display the debug manager
			m_debugManager.displayMenu();

			// Render
			m_window.clear(255, 255, 255);

			m_stateManager.render();
			m_debugManager.render();

			m_window.display();
			frames++;
			
			if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timer) > 1000)
			{
				timer += 1000;

				std::string header = m_appTitle + "  |  " + std::to_string(updates) + " ups, " + std::to_string(frames) + " fps";

                RE_LOG(LogLevel::INFO, header, "Application::run", "Application.cpp", 155);
				m_window.setTitle(header);

				updates = 0;
				frames = 0;
			}
		}

		al_stop_timer(l_timer);
		al_destroy_timer(l_dt);
		al_destroy_timer(l_timer);

		m_stateManager.unload();

		RE_LOG(LogLevel::INFO, "Program quit successfully", "Application::run", "Application.cpp", 169);
		RE_LOG_SAVE(m_saveLog);

		return EXIT_SUCCESS;
	}
}
