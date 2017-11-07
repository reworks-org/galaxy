//
//  Window.cpp
//  rework
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <algorithm>

#include <allegro5/timer.h>
#include <allegro5/mouse.h>
#include <allegro5/drawing.h>
#include <allegro5/keyboard.h>
#include <allegro5/bitmap_io.h>
#include <allegro5/bitmap_draw.h>

#include "re/services/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "Window.hpp"

// https://wiki.allegro.cc/index.php?title=Achieving_Resolution_Independence

namespace re
{
	Window::Window(int width, int height, bool fullscreen, bool msaa, int msaaValue, const std::string& title, const std::string& icon)
	:m_running(true), m_fullscreen(fullscreen)
	{
		al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL_3_0 | ALLEGRO_PROGRAMMABLE_PIPELINE);
		al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, msaa, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_SAMPLES, msaaValue, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_RENDER_METHOD, 1, ALLEGRO_REQUIRE);
		al_set_new_display_option(ALLEGRO_UPDATE_DISPLAY_REGION, 1, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_SUPPORT_NPOT_BITMAP, 1, ALLEGRO_REQUIRE);
		al_set_new_display_option(ALLEGRO_CAN_DRAW_INTO_BITMAP, 1, ALLEGRO_REQUIRE);

		// Max 255 characters
		al_set_new_window_title(title.c_str());

		m_display = al_create_display(width, height);
		if (!m_display)
		{
			LOG_S(FATAL) << "Could not create a display!" << std::endl;
		}

		if (m_fullscreen == true)
		{
			al_set_display_flag(m_display, ALLEGRO_FULLSCREEN_WINDOW, true);
		}

		// allegro requires null over nullptr here.
		m_icon = al_load_bitmap(icon.c_str());
		al_set_display_icon(m_display, m_icon);

		al_get_display_mode(al_get_num_display_modes() - 1, &m_displayData);
		
		int sx = width / m_displayData.width;
		int sy = height / m_displayData.height;
		int scale = std::min(sx, sy);

		m_fullscreenScale.width = m_displayData.width * scale;
		m_fullscreenScale.height = m_displayData.height * scale;
		m_fullscreenScale.x = (width - m_fullscreenScale.width) / 2;
		m_fullscreenScale.y = (height - m_fullscreenScale.height) / 2;

		LOG_S(INFO) << "width: " << m_displayData.width << " height: " << m_displayData.height << std::endl;

		m_fullscreenBuffer = al_create_bitmap(m_displayData.width, m_displayData.height);
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
			al_draw_scaled_bitmap(m_fullscreenBuffer, 0, 0, m_displayData.width, m_displayData.height, m_fullscreenScale.x, m_fullscreenScale.y, m_fullscreenScale.width, m_fullscreenScale.height, 0);
		}
		
		al_flip_display();
	}

	ALLEGRO_DISPLAY* Window::getDisplay()
	{	
		return m_display;
	}
}