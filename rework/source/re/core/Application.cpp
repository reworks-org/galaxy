//
//  Application.cpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

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
#include "re/utility/Time.hpp"

#include <chrono>
#include <iostream>

// https://github.com/alecthomas/entityx/blob/master/examples/example.cc

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
		double timePerFrame = 1.0 / 60.0;

		ALLEGRO_TIMER* clock = al_create_timer(timePerFrame);
		ALLEGRO_EVENT event;
		ALLEGRO_EVENT_QUEUE* eq = al_create_event_queue();

		al_register_event_source(eq, al_get_timer_event_source(clock));
		al_register_event_source(eq, al_get_keyboard_event_source());

		m_stateManager.load();
		
		al_start_timer(clock);
		while (m_window.isOpen());
		{
			while (al_get_next_event(eq, &event))
			{
				switch (event.type)
				{
				case ALLEGRO_EVENT_TIMER:
					break;

				case ALLEGRO_EVENT_KEY_DOWN:
					running = false;
					break;
				}
			}
                   // switch(l_event.type)
                    //{
					//case ALLEGRO_EVENT_DISPLAY_CLOSE:
					//	m_window.close();
					//	break;
                //    }
                    
                    //m_debugManager.event(&l_event);
            //    }
                
               // m_stateManager.event(&l_event);
                
				// Update
				//m_stateManager.update(l_dt);
				//m_debugManager.update();
            
			// Display the debug manager
			//m_debugManager.displayMenu();

			// Render
			//m_window.clear(255, 255, 255);

			//m_stateManager.render();
			//m_debugManager.render();

			//m_window.display();
		}

		al_stop_timer(clock);
		al_destroy_timer(clock);
		al_destroy_event_queue(eq);

		//m_stateManager.unload();

		//RE_LOG(LogLevel::INFO, "Program quit successfully", "Application::run", "Application.cpp", 169);
		//RE_LOG_SAVE(m_saveLog);

		return EXIT_SUCCESS;
	}
}
