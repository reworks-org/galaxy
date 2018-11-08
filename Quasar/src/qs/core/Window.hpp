///
/// Window.hpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_WINDOW_HPP_
#define QUASAR_WINDOW_HPP_

#include <string>

#include <SDL2/SDL.h>

namespace qs
{
	///
	/// Represents a window to open, process and draw to.
	///
	class Window final
	{
	public:
		///
		/// Default constructed window.
		///
		Window();

		///
		/// \brief Construct a Window.
		///
		/// Shortcut to calling create(). Call after SDL_Init(SDL_INIT_VIDEO) or SDL_Init(SDL_INIT_EVERYTHING).
		///
		/// \param title Title of the window.
		/// \param w Width of the window.
		/// \param h Height of the window.
		/// \param windowFlags SDL2 Window flags. You do not need to specify SDL_WINDOW_OPENGL or SDL_WINDOW_SHOWN.
		///
		explicit Window(const std::string& title, int w, int h, Uint32 windowFlags = 0) noexcept;

		///
		/// \brief Destroys SDL Window and OpenGL context.
		///
		/// Shortcut of calling destroy(). Make sure SDL_Quit has not been called yet.
		///
		~Window() noexcept;

		///
		/// \brief Construct a Window.
		///
		/// Call after SDL_Init(SDL_INIT_VIDEO) or SDL_Init(SDL_INIT_EVERYTHING).
		///
		/// \param title Title of the window.
		/// \param w Width of the window.
		/// \param h Height of the window.
		/// \param windowFlags SDL2 Window flags.
		///
		/// \return Returns true on success, false on failure.
		///
		bool create(const std::string& title, int w, int h, Uint32 windowFlags = 0) noexcept;

		///
		/// \brief Destroys SDL Window and OpenGL context.
		///
		/// Make sure SDL_Quit has not been called yet.
		///
		void destroy();

		///
		/// Checks if window is currently open or not.
		///
		/// \return Returns true if window is currently open, false if not.
		///
		bool isOpen() const;

		///
		/// \brief Closes the current window.
		///
		/// Internally, sets isOpen to false.
		///
		void close();
		
		///
		/// \brief Resize the Window and GL context.
		///
		/// You need to call this when the window resize event is triggered.
		///
		/// \param w New width of the window.
		/// \param h new height of the window.
		///
		void resize(int w, int h);

		///
		/// Clears the Rendering buffer.
		///
		void clear();

		///
		/// \brief Renders the OpenGL buffer to the screen.
		///
		/// Basically calls SDL_GL_SwapWindow().
		///
		void swap();

		///
		/// Retrieve pointer to SDL_Window object.
		///
		/// \return Returns const pointer to SDL_Window.
		///
		SDL_Window* getWindow() const noexcept;

		///
		/// Retrieve gl context.
		///
		/// \return Reference to GL context;
		///
		SDL_GLContext& getContext() const noexcept;

	private:
		///
		/// Keeps track of window state.
		///
		bool m_isOpen;

		///
		/// SDL window data.
		///
		SDL_Window* m_window;

		///
		/// OpenGL Context.
		///
		SDL_GLContext m_glContext;
	};
}

#endif