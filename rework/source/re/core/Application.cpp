//
//  Application.cpp
//  rework
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
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
#include "re/utility/Time.hpp"

#include "Application.hpp"

namespace re
{
	Application::Application()
	{
		Log::init();
		std::srand(std::time(nullptr));

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
		int frames = 0;
		int updates = 0;
		double timePerFrame = 1.0 / 60.0;

		ALLEGRO_TIMER* clock = al_create_timer(timePerFrame);
		ALLEGRO_EVENT event;

		al_register_event_source(eq, al_get_timer_event_source(clock));
		al_register_event_source(eq, al_get_keyboard_event_source());
		
		std::uint64_t timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		al_start_timer(clock);

		// load state?

		while (m_window.isOpen());
		{
			while (al_get_next_event(eq, &event))
			{
				switch (event.type)
				{
				case ALLEGRO_EVENT_TIMER:
					// event
					// update
					updates++;	
					break;

				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					running = false;
					break;
				}
			}

			// render
			frames++;

			if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timer) > 1000)
			{
				timer += 1000;

				std::string header = m_appTitle + "  |  " + std::to_string(updates) + " ups, " + std::to_string(frames) + " fps";

				BOOST_LOG_TRIVIAL(logging::trivial::info) << header;

				updates = 0;
				frames = 0;
			}
		}

		// unload state

		al_stop_timer(clock);
		al_destroy_timer(clock);
		al_destroy_event_queue(eq);
		BOOST_LOG_TRIVIAL(logging::trivial::info) << "Application close.";

		return EXIT_SUCCESS;
	}
}
