///
/// Window.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_WINDOW_HPP_
#define QUASAR_WINDOW_HPP_

#include <string>

#include <SDL2/SDL_video.h>
#include <protostar/system/Colour.hpp>

///
/// Core namespace.
///
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
		Window() noexcept;

		///
		/// \brief Window creation constructor.
		///
		/// Calls create().
		/// Call after SDL_Init(SDL_INIT_VIDEO) or SDL_Init(SDL_INIT_EVERYTHING).
		///
		/// \param title Title of the window.
		/// \param w Width of the window.
		/// \param h Height of the window.
		/// \param windowFlags SDL2 Window flags.
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
		void destroy() noexcept;

		///
		/// Checks if window is currently open or not.
		///
		/// \return Returns true if window is currently open, false if not.
		///
		bool isOpen() const noexcept;

		///
		/// \brief Closes the current window.
		///
		/// Internally, sets isOpen to false.
		///
		void close() noexcept;
		
		///
		/// \brief Resizes the OpenGL viewport.
		///
		/// You need to call this when the window resize event is triggered.
		/// Example: 
		///
		///	case SDL_WINDOWEVENT_SIZE_CHANGED:
		///      onResize(event->window.data1, event->window.data2);
		///      break;
		///
		/// \param w New width of the window.
		/// \param h new height of the window.
		///
		void onSizeChanged(int w, int h) noexcept;

		///
		/// Resizes window and gl viewport.
		///
		/// \param w New width of the window.
		/// \param h new height of the window.
		///
		void resize(int w, int h) noexcept;

		///
		/// Clears the Rendering buffer.
		///
		/// \param colour Colour to clear to.
		///
		void clear(const protostar::Colour& colour) noexcept;

		///
		/// \brief Renders the OpenGL buffer to the screen.
		///
		/// Basically calls SDL_GL_SwapWindow().
		///
		void swap() noexcept;

		///
		/// Retrieve pointer to SDL_Window object.
		///
		/// \return Returns const pointer to SDL_Window.
		///
		SDL_Window* getWindow() noexcept;

		///
		/// Retrieve gl context.
		///
		/// \return Reference to GL context;
		///
		SDL_GLContext& getContext() noexcept;

		///
		/// Get window width.
		///
		/// \return Const integer.
		///
		const int getWidth() const noexcept;

		///
		/// Get window height.
		///
		/// \return Const integer.
		///
		const int getHeight() const noexcept;

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

		///
		/// Window width.
		///
		int m_width;
		
		///
		/// Window height.
		///
		int m_height;
	};
}

#endif