///
/// Window.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_WINDOW_HPP_
#define GALAXY_CORE_WINDOW_HPP_

#include <span>
#include <string_view>

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <robin_hood.h>

#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/core/WindowSettings.hpp"
#include "galaxy/events/dispatcher/Dispatcher.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowResized.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/texture/RenderTexture.hpp"
#include "galaxy/input/Keys.hpp"
#include "galaxy/input/Mouse.hpp"

namespace galaxy
{
	namespace graphics
	{
		class Renderer;
	} // namespace graphics

	namespace core
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
			struct Cursor final
			{
				///
				/// Constructor.
				///
				Cursor() noexcept;

				///
				/// Destructor.
				///
				~Cursor() noexcept = default;

				///
				/// Allows for custom cursor properties.
				///
				GLFWcursor* m_glfw;

				///
				/// Stores current position.
				///
				glm::dvec2 m_pos;

				///
				/// Stores previous position.
				///
				glm::dvec2 m_prev_pos;
			};

			///
			/// Scroll event data.
			///
			struct ScrollData final
			{
				///
				/// Constructor.
				///
				ScrollData() noexcept;

				///
				/// Is the user currently scrolling.
				///
				bool m_active;

				///
				/// Mouse wheel offsets.
				///
				events::MouseWheel m_data;
			};

			///
			/// Constructor.
			///
			Window() noexcept;

			///
			/// \brief Window creation constructor.
			///
			/// Calls create().
			///
			/// \param settings Settings of the window.
			///
			Window(const WindowSettings& settings);

			///
			/// \brief Destroys SDL Window and OpenGL context.
			///
			/// Shortcut of calling destroy(). Make sure SDL_Quit has not been called yet.
			///
			~Window();

			///
			/// Construct a Window.
			///
			/// \param settings Settings of the window.
			///
			[[nodiscard]] const bool create(const WindowSettings& settings);

			///
			/// Sets the background colour of the window.
			///
			/// \param col Colour to use.
			///
			void set_window_background(graphics::Colour& col) noexcept;

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
			[[nodiscard]] const bool is_open() const noexcept;

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
			///
			void resize(const int width, const int height);

			///
			/// \brief Notify's user of an event without interrupting.
			///
			/// Think like windows flashing tray icon.
			///
			void request_attention() noexcept;

			///
			/// \brief Allow the window to be closed by the OS.
			///
			/// I.E from ALT+F4 or the close button in the window frame.
			///
			void allow_native_closing() noexcept;

			///
			/// \brief Prevent the window from being closed by the OS.
			///
			/// I.E from ALT+F4 or the close button in the window frame.
			///
			void prevent_native_closing() noexcept;

			///
			/// Clears the Rendering buffer.
			///
			void begin();

			///
			/// Renders the OpenGL buffer to the screen.
			///
			void end();

			///
			/// Poll for events.
			///
			void poll_events() noexcept;

			///
			/// Check if a mouse button was pressed.
			///
			/// \param mouse_button mouse button keycode.
			///
			/// \return True if a press occured.
			///
			[[nodiscard]] const bool mouse_button_pressed(input::MouseButton mouse_button) noexcept;

			///
			/// Check if a mouse button was released.
			///
			/// \param mouse_button mouse button keycode.
			///
			/// \return True if a release occured.
			///
			[[nodiscard]] const bool mouse_button_released(input::MouseButton mouse_button) noexcept;

			///
			/// \brief Set scroll data.
			///
			/// Called internally for you.
			///
			/// \param active Is the user currently scrolling?
			/// \param x -1 or 1.
			/// \param y -1 or 1.
			///
			void set_scroll_data(const bool active, const int x, const int y);

			///
			/// Check if user is currently scrolling.
			///
			/// \return True if user if scroll callback is being called.
			///
			[[nodiscard]] const bool is_scrolling() const noexcept;

			///
			/// \brief Get scroll data.
			///
			/// This sets is_scrolling to false, so you need to check is_scrolling first!
			///
			/// \return Const reference to event data.
			///
			[[nodiscard]] const events::MouseWheel& get_scroll_data();

			///
			/// Register an object with the window dispatch resizer.
			///
			/// \param obj An object with an on_event(const WindowResized& event) function.
			///
			template<meta::is_class Type>
			void register_on_window_resize(Type& obj) noexcept;

			///
			/// \brief See if a key is being held down.
			///
			/// This will pick up repeated events.
			///
			/// \param key Key to check.
			///
			/// \return True if key is currently down.
			///
			[[nodiscard]] const bool key_down(input::Keys key) noexcept;

			///
			/// Check if a key was pressed once.
			///
			/// \param key Key to check.
			///
			/// \return True if key was pressed once.
			///
			[[nodiscard]] const bool key_pressed(input::Keys key) noexcept;

			///
			/// Starts filling the returned pointer to string with characters as they are typed.
			///
			/// \return Pointer to std::string that is updated with characters.
			///
			[[nodiscard]] std::string* begin_text_input() noexcept;

			///
			/// Stop updating strings with text input.
			///
			void end_text_input() noexcept;

			///
			/// Get current cursor position.
			///
			/// \return Returns true or false if cursor has moved, and the current position.
			///
			[[nodiscard]] std::tuple<bool, glm::vec2> get_cursor_pos() noexcept;

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

			///
			/// Get cursor size.
			///
			/// \return Const glm::vec2.
			///
			[[nodiscard]] const glm::vec2& cursor_size() const noexcept;

		private:
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
			/// Map of GLFW mouse buttons to galaxy mouse buttons.
			///
			robin_hood::unordered_map<input::MouseButton, int> m_mousebutton_map;

			///
			/// Map of GLFW scancodes to galaxy keys.
			///
			robin_hood::unordered_map<input::Keys, int> m_keymap;

			///
			/// Previous key states.
			///
			robin_hood::unordered_map<input::Keys, int> m_prev_key_states;

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
			std::unique_ptr<graphics::RenderTexture> m_framebuffer;

			///
			/// Internal framebuffer VAO.
			///
			std::unique_ptr<components::Sprite> m_fb_sprite;

			///
			/// Internal framebuffer transform.
			///
			components::Transform m_fb_transform;

			///
			/// Triggers window resized event.
			///
			events::Dispatcher m_window_dispatcher;

			///
			/// Cursor size.
			///
			glm::vec2 m_cursor_size;

			///
			/// Scroll data.
			///
			ScrollData m_scroll_data;
		};

		template<meta::is_class Type>
		inline void galaxy::core::Window::register_on_window_resize(Type& obj) noexcept
		{
			m_window_dispatcher.subscribe<events::WindowResized, Type>(obj);
		}
	} // namespace core
} // namespace galaxy

#endif