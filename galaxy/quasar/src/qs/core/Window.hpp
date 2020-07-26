///
/// Window.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_WINDOW_HPP_
#define QUASAR_WINDOW_HPP_

#include <string_view>

#include <glfw/glfw3.h>
#include <protostar/graphics/Colour.hpp>

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
		explicit Window(std::string_view title, const int w, const int h);

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
		bool create(std::string_view title, const int w, const int h);

		///
		/// Sets the background colour of the window.
		///
		/// \param col Colour to use.
		///
		void set_window_background(pr::Colour& col);

		///
		/// Set window icon.
		///
		/// \param icon Icon to load.
		///
		void set_icon(std::string_view icon);

		///
		/// Set window icon.
		///
		/// \param mem Memory buffer containing pixels.
		/// \param size Size of the buffer.
		///
		void set_icon(pr::not_nullptr auto mem, const int size);

		///
		/// Set cursor visibility.
		///
		/// \param visible True for cursor to be visible, otherwise false.
		///
		void set_cursor_visibility(const bool visible) noexcept;

		///
		/// \brief Restricts cursor to window bounds, and makes invisible.
		///
		/// setCursorVisibility() overrides this.
		///
		void remove_cursor() noexcept;

		///
		/// Set cursor icon.
		///
		/// \param icon Icon to load.
		///
		void set_cursor_icon(std::string_view icon);

		///
		/// Set cursor icon.
		///
		/// \param mem Memory buffer containing pixels.
		/// \param size Size of the buffer.
		///
		void set_cursor_icon(pr::not_nullptr auto mem, const int size);

		///
		/// Set the callback function for scrolling.
		///
		/// \param func Function callback.
		///
		void set_scrolling_callback(GLFWscrollfun func) noexcept;

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
		bool is_open() const noexcept;

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
		void request_attention() noexcept;

		///
		/// Clears the Rendering buffer.
		///
		void begin() noexcept;

		///
		/// \brief Renders the OpenGL buffer to the screen.
		///
		/// Basically calls SDL_GL_SwapWindow().
		///
		void end() noexcept;

		///
		/// Poll for events.
		///
		void poll_events() noexcept;

		///
		/// Retrieve pointer to GLFWwindow object.
		///
		/// \return Returns const pointer to GLFWwindow.
		///
		GLFWwindow* gl_window() noexcept;

		///
		/// Get window width.
		///
		/// \return Const integer.
		///
		const int get_width() const noexcept;

		///
		/// Get window height.
		///
		/// \return Const integer.
		///
		const int get_height() const noexcept;

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

		///
		/// Window background colour.
		///
		std::array<float, 4> m_colour;
	};
} // namespace qs

#endif