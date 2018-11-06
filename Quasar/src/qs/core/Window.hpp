///
/// Window.hpp
/// Quasar
///
/// Created by reworks on 16/09/2018.
/// MIT LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_WINDOW_HPP_
#define QUASAR_WINDOW_HPP_

#include <string>

#include <SDL2/SDL_render.h>

namespace qs
{
	///
	/// Represents a window to open, process and draw to.
	///
	class Window final
	{
	public:
		///
		/// Construct a window.
		///
		/// \param title Title of the window.
		/// \param w Width of the window.
		/// \param h Height of the window.
		/// \param windowFlags SDL2 Window flags.
		/// \param renderFlags SDL2 Renderer flags.
		///
		explicit Window(const std::string& title, int w, int h, Uint32 windowFlags = 0, Uint32 renderFlags = 0);

		///
		/// Clean up class data.
		///
		~Window();

		///
		/// Retrieve our renderer.
		///
		/// \return CONST pointer to SDL_Renderer object. We want it const because we don't want the pointer to change.
		///
		SDL_Renderer* getRenderer() const noexcept;

	public:
		///
		/// Keep track of window state.
		///
		bool m_isOpen;

	private:
		///
		/// Pointer to window data.
		///
		SDL_Window* m_window;

		///
		/// Pointer to renderer data.
		///
		SDL_Renderer* m_renderer;
	};
}

#endif