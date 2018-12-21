///
/// Window.cpp
/// starlight
///
/// Created by reworks on 19/09/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/error.h>
#include <allegro5/mouse.h>
#include <allegro5/monitor.h>
#include <allegro5/drawing.h>
#include <allegro5/bitmap_io.h>
#include <allegro5/bitmap_draw.h>

#include "sl/libs/loguru/loguru.hpp"

#include "Window.hpp"

namespace sl
{
	Window::Window(const std::string& title, const std::string& icon, const int width, const int height, const bool fullscreen, const bool msaa, const int msaaValue, const bool grabMouse)
		:m_isOpen(true), m_fullscreen(fullscreen), m_icon(nullptr), m_display(nullptr), m_fullscreenBuffer(nullptr), m_size(width, height)
	{
		// Set the display options for the window based off of the params from the config.
		al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_OPENGL | ALLEGRO_PROGRAMMABLE_PIPELINE);
		al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, msaa, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_SAMPLES, msaaValue, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_RENDER_METHOD, 1, ALLEGRO_REQUIRE);
		al_set_new_display_option(ALLEGRO_UPDATE_DISPLAY_REGION, 1, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_SUPPORT_NPOT_BITMAP, 1, ALLEGRO_REQUIRE);
		al_set_new_display_option(ALLEGRO_CAN_DRAW_INTO_BITMAP, 1, ALLEGRO_REQUIRE);

		// Make sure title is the correct length.
		if (title.size() > 255)
		{
			std::string temp = title.substr(0, 255);
			LOG_S(WARNING) << "Title longer than 256 chars, shrinking to: " << temp;
			al_set_new_window_title(temp.c_str());
		}
		else
		{
			al_set_new_window_title(title.c_str());
		}
		
		// Create the display making sure it was successful.
		m_display = al_create_display(width, height);
		if (!m_display)
		{
			LOG_S(FATAL) << "Could not create a display!";
		}

		// Set fullscreen flag.
		if (!al_set_display_flag(m_display, ALLEGRO_FULLSCREEN_WINDOW, m_fullscreen))
		{
			LOG_S(ERROR) << "Failed to set fullscreen flag!";
		}

		// Set mouse grab flag.
		if (grabMouse)
		{
			if (!al_grab_mouse(m_display))
			{
				LOG_S(ERROR) << "Failed to grab display mouse.";
			}
		}

		// Set window icon.
		m_icon = al_load_bitmap(icon.c_str());
		if (m_icon)
		{
			al_set_display_icon(m_display, m_icon);
		}
		else
		{
			LOG_S(ERROR) << "Allegro failed to load icon! Errno: " << al_get_errno();
		}

		// We get the monitor info in order to determine the correct size of the fullscreen window.
		ALLEGRO_MONITOR_INFO info;
		al_get_monitor_info(0, &info);
		
		// Set the scale of the fullscreen using the window info.
		// Makes sure the game is rendered in the middle of the screen in fullscreen mode, if the window is smaller than the fullscreen dimensions.
		m_fullscreenScale.m_x = ((info.x2 - info.x1) / 2) - (width / 2);
		m_fullscreenScale.m_y = ((info.y2 - info.y1) / 2) - (height / 2);

		m_fullscreenBuffer = al_create_bitmap(width, height);
		if (!m_fullscreenBuffer)
		{
			LOG_S(FATAL) << "Allegro could not create fullscreen buffer! Errno: " << al_get_errno();
		}
	}
	
	Window::~Window()
	{	
		// Check to make sure we are not freeing nullptrs.

		if (m_icon)
		{
			al_destroy_bitmap(m_icon);
		}

		if (m_fullscreenBuffer)
		{
			al_destroy_bitmap(m_fullscreenBuffer);
		}

		if (m_display)
		{
			al_destroy_display(m_display);
		}
	}

	void Window::setTitle(const std::string& newTitle)
	{
		if (!newTitle.empty())
		{
			al_set_window_title(m_display, newTitle.c_str());
		}
	}

	void Window::toggleFullscreen()
	{
		// Toggle fullscreen mode, updating relevant variables.
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
		m_isOpen = false;
	}

	bool Window::isOpen() const
	{
		return m_isOpen;
	}

	void Window::clear(unsigned int r, unsigned int g, unsigned int b) const
	{
		// When rendering in fullscreen mode, we have to draw to a buffer first.
		if (m_fullscreen)
		{
			al_set_target_bitmap(m_fullscreenBuffer);
		}

		al_clear_to_color(al_map_rgba(r, g, b, 0));
	}

	void Window::display() const
	{
		if (m_fullscreen)
		{
			// Then that buffer is drawn to the middle of the screen. This way we don't have to individually offset every draw call.
			al_set_target_bitmap(al_get_backbuffer(m_display));
			al_clear_to_color(al_map_rgba(0, 0, 0, 0));
			al_draw_bitmap(m_fullscreenBuffer, m_fullscreenScale.m_x, m_fullscreenScale.m_y, 0);
		}
		
		al_flip_display();
	}

	ALLEGRO_DISPLAY* Window::getDisplay() const
	{
		return m_display;
	}

	const Vector2<int>& Window::getSize() const
	{
		return m_size;
	}
}