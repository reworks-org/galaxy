///
/// Window.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_WINDOW_HPP_
#define QUASAR_WINDOW_HPP_

#include <span>
#include <string_view>

#include <glfw/glfw3.h>
#include <glm/vec2.hpp>
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
		/// Represents a window cursor.
		///
		struct Cursor
		{
			///
			/// Allows for custom cursor properties.
			///
			GLFWcursor* m_glfw = nullptr;

			///
			/// Stores current position.
			///
			glm::dvec2 m_pos = {0.0f, 0.0f};

			///
			/// Stores previous position.
			///
			glm::dvec2 m_prev_pos = {0.0f, 0.0f};
		};

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
		/// \param width Width of the window.
		/// \param height Height of the window.
		///
		explicit Window(std::string_view title, const int width, const int height);

		///
		/// Copy constructor.
		///
		Window(const Window&) noexcept = delete;

		///
		/// Move constructor.
		///
		Window(Window&&) noexcept = delete;

		///
		/// Copy assignment operator.
		///
		Window& operator=(const Window&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		Window& operator=(Window&&) noexcept = delete;

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
		/// \param width Width of the window.
		/// \param height Height of the window.
		/// \param settings Settings for the window, like flags and MSAA.
		///
		/// \return Returns true on success, false on failure.
		///
		bool create(std::string_view title, const int width, const int height);

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
		/// \param buffer Memory buffer containing pixels.
		///
		void set_icon(std::span<unsigned char> buffer);

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
		/// \param buffer Memory buffer containing pixels.
		///
		void set_cursor_icon(std::span<unsigned char> buffer);

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
		[[nodiscard]] bool is_open() const noexcept;

		///
		/// \brief Closes the current window.
		///
		/// Internally, sets isOpen to false.
		///
		void close() noexcept;

		///
		/// Resizes window.
		///
		/// \param width Width of the window.
		/// \param height Height of the window.
		/// \param window Pointer to window to resize. Defaults to *this.
		///
		void resize(const int width, const int height) noexcept;

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
		/// Check if a mouse button was pressed.
		///
		/// \param mouse_button GLFW mouse button keycode.
		///
		/// \return True if a press occured.
		///
		bool mouse_button_pressed(int mouse_button);

		///
		/// Check if a mouse button was released.
		///
		/// \param mouse_button GLFW mouse button keycode.
		///
		/// \return True if a release occured.
		///
		bool mouse_button_released(int mouse_button);

		///
		/// Get current cursor position.
		///
		/// \return Returns true or false if cursor has moved, and the current position.
		///
		std::tuple<bool, glm::vec2> get_cursor_pos();

		///
		/// Retrieve pointer to GLFWwindow object.
		///
		/// \return Returns const pointer to GLFWwindow.
		///
		[[nodiscard]] GLFWwindow* gl_window() noexcept;

		///
		/// Get window width.
		///
		/// \return Const integer.
		///
		[[nodiscard]] const int get_width() const noexcept;

		///
		/// Get window height.
		///
		/// \return Const integer.
		///
		[[nodiscard]] const int get_height() const noexcept;

	private:
		///
		/// GLFW window data.
		///
		GLFWwindow* m_window;

		///
		/// Custom cursor.
		///
		Cursor m_cursor;

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

		///
		/// Array storing each of the 8 mouse buttons supported by GLFW.
		///
		std::array<int, 8> m_prev_mouse_btn_states;
	};
} // namespace qs

#endif