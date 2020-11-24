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

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <protostar/graphics/Colour.hpp>
#include <protostar/system/Keys.hpp>
#include <robin_hood.h>

#include "qs/sprite/Sprite.hpp"
#include "qs/texture/RenderTexture.hpp"

///
/// Core namespace.
///
namespace qs
{
	class Renderer;

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
		Window();

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
		Window(const Window&) = delete;

		///
		/// Move constructor.
		///
		Window(Window&&) = delete;

		///
		/// Copy assignment operator.
		///
		Window& operator=(const Window&) = delete;

		///
		/// Move assignment operator.
		///
		Window& operator=(Window&&) = delete;

		///
		/// \brief Destroys SDL Window and OpenGL context.
		///
		/// Shortcut of calling destroy(). Make sure SDL_Quit has not been called yet.
		///
		~Window();

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
		void set_cursor_visibility(const bool visible);

		///
		/// \brief Restricts cursor to window bounds, and makes invisible.
		///
		/// setCursorVisibility() overrides this.
		///
		void remove_cursor();

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
		void set_scrolling_callback(GLFWscrollfun func);

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
		[[nodiscard]] bool is_open() const;

		///
		/// \brief Closes the current window.
		///
		/// Internally, sets isOpen to false.
		///
		void close();

		///
		/// Resizes window.
		///
		/// \param width Width of the window.
		/// \param height Height of the window.
		/// \param window Pointer to window to resize. Defaults to *this.
		///
		void resize(const int width, const int height);

		///
		/// \brief Notify's user of an event without interrupting.
		///
		/// Think like windows flashing tray icon.
		///
		void request_attention();

		///
		/// Clears the Rendering buffer.
		///
		void begin();

		///
		/// Renders the OpenGL buffer to the screen.
		///
		/// \param renderer Renderer used to output to screen.
		///
		void end(qs::Renderer* renderer);

		///
		/// Poll for events.
		///
		void poll_events();

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
		/// \brief See if a key is being held down.
		///
		/// This will pick up repeated events.
		///
		/// \param key Key to check.
		///
		/// \return True if key is currently down.
		///
		bool key_down(pr::Keys key);

		///
		/// Check if a key was pressed once.
		///
		/// \param key Key to check.
		///
		/// \return True if key was pressed once.
		///
		bool key_pressed(pr::Keys key);

		///
		/// Starts filling the returned pointer to string with characters as they are typed.
		///
		/// \return Pointer to std::string that is updated with characters.
		///
		std::string* begin_text_input();

		///
		/// Stop updating strings with text input.
		///
		void end_text_input();

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
		[[nodiscard]] GLFWwindow* gl_window();

		///
		/// Get window width.
		///
		/// \return Const integer.
		///
		[[nodiscard]] const int get_width() const;

		///
		/// Get window height.
		///
		/// \return Const integer.
		///
		[[nodiscard]] const int get_height() const;

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

		///
		/// Map of GLFW scancodes to protostar keys.
		///
		robin_hood::unordered_map<pr::Keys, int> m_keymap;

		///
		/// Previous key states.
		///
		robin_hood::unordered_map<pr::Keys, int> m_prev_key_states;

		///
		/// String for text input.
		///
		std::string m_text_input;

		///
		/// Flag to signal to glfw that text is being input.
		///
		bool m_inputting_text;

		///
		/// Internal framebuffer.
		///
		std::unique_ptr<qs::RenderTexture> m_framebuffer;

		///
		/// Internal framebuffer sprite.
		///
		std::unique_ptr<qs::Sprite> m_fb_sprite;
	};
} // namespace qs

#endif