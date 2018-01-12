///
///  Window.hpp
///  starlight
///
///  Created by reworks on 19/09/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_WINDOW_HPP_
#define REWORK_WINDOW_HPP_

#include <string>
#include <allegro5/bitmap.h>
#include <allegro5/display.h>

#include "re/math/Vector2.hpp"
#include "re/types/ServiceLocator.hpp"

namespace sl
{
	class Window : public ServiceLocator<Window>
	{
	public:
		///
		/// \brief Sets up window.
		/// 
		/// Sets up all the data for the allegro window such as rendering.
		///
		/// \param width Window width.
		/// \param height Window height.
		/// \param fullscreen Should the window be fullscreen on startup, true or false.
		/// \param msaa Enable or disable MultiSample AntiAliasing.
		/// \param msaaValue Strength of MSAA. 2, 4, 8, 16 are valid values. Set to 0 if msaa is disabled.
		/// \param title Window title. Must be smaller than 255 characters.
		/// \param icon Path to window icon. Must be 512x512.
		///
		Window(int width, int height, bool fullscreen, bool msaa, int msaaValue, const std::string& title, const std::string& icon);

		///
		/// Cleans up the window.
		///
		~Window() override;

		///
		/// Change the title.
		///
		/// \param newTitle The new title string.
		///
		void setTitle(const std::string& newTitle);

		///
		/// Toggle window between fullscreen and windowed.
		///
		void toggleFullscreen();

		///
		/// Sets m_running to false, to close application.
		///
		void close();

		///
		/// See if the window is still running.
		///
		/// \return Const boolean.
		///
		bool isOpen() const;

		///
		/// Clears the window to an RGB colour.
		///
		/// \param r Red value.
		/// \param g Green value.
		/// \param b Blue value.
		///
		void clear(unsigned int r, unsigned int g, unsigned int b);

		///
		/// Display contents drawn to window.
		///
		void display();

		///
		/// Retrieves the internal display.
		///
		/// \return Returns pointer to ALLEGRO_DISPLAY.
		///
		ALLEGRO_DISPLAY* getDisplay();

		///
		/// Retrieves the window size.
		///
		/// \return Returns CONST vector2 of type int.
		///
		const Vector2<int>& getSize() const;

	private:
		bool m_running;
		bool m_fullscreen;
		ALLEGRO_BITMAP* m_icon;
		ALLEGRO_DISPLAY* m_display;
		ALLEGRO_BITMAP* m_fullscreenBuffer;
		Vector2<int> m_size;
		Vector2<int> m_fullscreenScale;

	private:
		///
		/// Copy Constructor.
		/// Deleted.
		///
		Window(const Window&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		Window(Window&&) = delete;
	};
}

#endif