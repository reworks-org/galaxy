//
//  Window.cpp
//  starlight
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <algorithm>
#include <allegro5/timer.h>
#include <allegro5/mouse.h>
#include <allegro5/monitor.h>
#include <allegro5/drawing.h>
#include <allegro5/keyboard.h>
#include <allegro5/bitmap_io.h>
#include <allegro5/bitmap_draw.h>

#include "re/fs/VFS.hpp"

#include "Window.hpp"

namespace sl
{
	Window::Window(int width, int height, bool fullscreen, bool msaa, int msaaValue, const std::string& title, const std::string& icon)
	:m_running(true), m_fullscreen(fullscreen), m_size(width, height)
	{
		al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL_3_0 | ALLEGRO_PROGRAMMABLE_PIPELINE);
		al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, msaa, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_SAMPLES, msaaValue, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_RENDER_METHOD, 1, ALLEGRO_REQUIRE);
		al_set_new_display_option(ALLEGRO_UPDATE_DISPLAY_REGION, 1, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_SUPPORT_NPOT_BITMAP, 1, ALLEGRO_REQUIRE);
		al_set_new_display_option(ALLEGRO_CAN_DRAW_INTO_BITMAP, 1, ALLEGRO_REQUIRE);

		if (title.size() >= 255)
		{
			std::string temp = title.substr(0, 255);
			al_set_new_window_title(temp.c_str());
		}
		else
		{
			al_set_new_window_title(title.c_str());
		}

		m_display = al_create_display(width, height);
		if (!m_display)
		{
			LOG_S(FATAL) << "Could not create a display!";
		}

		if (m_fullscreen == true)
		{
			al_set_display_flag(m_display, ALLEGRO_FULLSCREEN_WINDOW, true);
		}

		m_icon = al_load_bitmap(icon.c_str());
		al_set_display_icon(m_display, m_icon);

		ALLEGRO_MONITOR_INFO info;
		al_get_monitor_info(0, &info);
		
		m_fullscreenScale.x = ((info.x2 - info.x1) / 2) - (width / 2);
		m_fullscreenScale.y = ((info.y2 - info.y1) / 2) - (height / 2);

		m_fullscreenBuffer = al_create_bitmap(width, height);
	}
	
	Window::~Window()
	{
		al_destroy_bitmap(m_icon);
		al_destroy_bitmap(m_fullscreenBuffer);
		al_destroy_display(m_display);
	}

	void Window::setTitle(const std::string& newTitle)
	{
		al_set_window_title(m_display, newTitle.c_str());
	}

	void Window::toggleFullscreen()
	{
		if (m_fullscreen == false)
		{
			m_fullscreen = true;
			al_set_display_flag(m_display, ALLEGRO_FULLSCREEN_WINDOW, true);
		}
		else
		{
			m_fullscreen = false;
			al_set_display_flag(m_display, ALLEGRO_FULLSCREEN_WINDOW, false);
		}
	}

	void Window::close()
	{
		m_running = false;
	}

	bool Window::isOpen() const
	{
		return m_running;
	}

	void Window::clear(unsigned int r, unsigned int g, unsigned int b)
	{
		if (m_fullscreen)
		{
			al_set_target_bitmap(m_fullscreenBuffer);
		}

		al_clear_to_color(al_map_rgb(r, g, b));
	}

	void Window::display()
	{
		if (m_fullscreen)
		{
			al_set_target_bitmap(al_get_backbuffer(m_display));
			al_clear_to_color(al_map_rgba(0, 0, 0, 255));
			al_draw_bitmap(m_fullscreenBuffer, m_fullscreenScale.x, m_fullscreenScale.y, 0);
		}
		
		al_flip_display();
	}

	ALLEGRO_DISPLAY* Window::getDisplay()
	{
		return m_display;
	}

	const Vector2<int>& Window::getSize() const
	{
		return m_size;
	}
}