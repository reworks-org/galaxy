///
/// Window.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_WINDOW_HPP_
#define QUASAR_WINDOW_HPP_

#include <string>

#include <glfw/glfw3.h>
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
		///
		explicit Window(const std::string& title, int w, int h) noexcept;

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
		/// \param settings Settings for the window, like flags and MSAA.
		///
		/// \return Returns true on success, false on failure.
		///
		bool create(const std::string& title, int w, int h) noexcept;

		///
		/// Set window icon.
		///
		/// \param icon Icon to load.
		///
		void setIcon(const std::string& icon) noexcept;

		///
		/// Set window icon.
		///
		/// \param mem Memory buffer containing pixels.
		/// \param size Size of the buffer.
		///
		void setIcon(const unsigned char* mem, const int size) noexcept;

		///
		/// Set cursor visibility.
		///
		/// \param visible True for cursor to be visible, otherwise false.
		///
		void setCursorVisibility(const bool visible) noexcept;

		///
		/// \brief Restricts cursor to window bounds, and makes invisible.
		///
		/// setCursorVisibility() overrides this.
		///
		void removeCursor() noexcept;

		///
		/// Set cursor icon.
		///
		/// \param icon Icon to load.
		///
		void setCursorIcon(const std::string& icon) noexcept;

		///
		/// Set cursor icon.
		///
		/// \param mem Memory buffer containing pixels.
		/// \param size Size of the buffer.
		///
		void setCursorIcon(const unsigned char* mem, const int size) noexcept;

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
		/// Resizes window.
		///
		/// \param w New width of the window.
		/// \param h new height of the window.
		/// \param window Pointer to window to resize. Defaults to *this.
		///
		void resize(int w, int h) noexcept;

		///
		/// \brief Notify's user of an event without interrupting.
		///
		/// Think like windows flashing tray icon.
		///
		void requestAttention() noexcept;

		///
		/// Clears the Rendering buffer.
		///
		/// \param colour Colour to clear to.
		///
		void begin(const protostar::Colour& colour) noexcept;

		///
		/// \brief Renders the OpenGL buffer to the screen.
		///
		/// Basically calls SDL_GL_SwapWindow().
		///
		void end() noexcept;

		///
		/// Poll for events.
		///
		void pollEvents() noexcept;

		///
		/// Retrieve pointer to GLFWwindow object.
		///
		/// \return Returns const pointer to GLFWwindow.
		///
		GLFWwindow* getWindow() noexcept;

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
		/// GLFW window data.
		///
		GLFWwindow* m_window;

		///
		/// GLFW custom cursor icon.
		///
		GLFWcursor* m_cursor;

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