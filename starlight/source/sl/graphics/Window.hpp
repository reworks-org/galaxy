///
/// Window.hpp
/// starlight
///
/// Created by reworks on 19/09/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_WINDOW_HPP_
#define STARLIGHT_WINDOW_HPP_

#include <string>
#include <allegro5/display.h>

#include "sl/math/Vector2.hpp"

namespace sl
{
	///
	/// Window class that covers the app window and rendering.
	///
	class Window final
	{
	public:
		///
		/// \brief Sets up window.
		/// 
		/// Sets up all the data for the allegro window such as rendering.
		///
		/// \param title Window title. Must be smaller than 255 characters.
		/// \param icon Path to window icon. Must be 512x512.
		/// \param width Window width.
		/// \param height Window height.
		/// \param fullscreen Should the window be fullscreen on startup, true or false.
		/// \param msaa Enable or disable MultiSample AntiAliasing.
		/// \param msaaValue Strength of MSAA. 2, 4, 8, 16 are valid values. Set to 0 if msaa is disabled.
		/// \param grabMouse Should the window confine the mouse?
		///
		Window(const std::string& title, const std::string& icon, const int width, const int height, const bool fullscreen, const bool msaa, const int msaaValue, const bool grabMouse);

		///
		/// Cleans up the window.
		///
		~Window() noexcept;

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
		/// Sets m_isOpen to false, to close application.
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
		void clear(unsigned int r, unsigned int g, unsigned int b) const;

		///
		/// Display contents drawn to window.
		///
		void display() const;

		///
		/// Retrieves the internal display.
		///
		/// \return Returns pointer to ALLEGRO_DISPLAY.
		///
		ALLEGRO_DISPLAY* getDisplay() const;

		///
		/// Retrieves the window size.
		///
		/// \return Returns CONST vector2 of type int.
		///
		const Vector2<int>& getSize() const;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Window() = delete;

	private:
		///
		/// Keeps track if the window is still open or not. 
		///
		bool m_isOpen;
		
		///
		/// Keeps track if the window is in fullscreen mode or not.
		///
		bool m_fullscreen;

		///
		/// Bitmap image for window icon. 
		///
		ALLEGRO_BITMAP* m_icon;

		///
		/// Pointer to display data structure.
		///
		ALLEGRO_DISPLAY* m_display;

		///
		/// Bitmap used to draw to when in fullscreen mode.
		///
		ALLEGRO_BITMAP* m_fullscreenBuffer;

		///
		/// Size of the window in pixels.
		///
		Vector2<int> m_size;

		///
		/// Scale of fullscreen mode. Set by Window constructor for you.
		/// Used to determine where to render the window in the middle of the screen when fullscreen,
		/// if the window is smaller than the fullscreen dimensions.
		///
		Vector2<int> m_fullscreenScale;
	};
}

#endif