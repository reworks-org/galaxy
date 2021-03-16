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
			/// \brief Set current scene dispatcher.
			///
			/// Best set/unset when scene is pushed/popped.
			///
			/// \param dispatcher Pointer to scene dispatcher.
			///
			void set_scene_dispatcher(events::Dispatcher* dispatcher) noexcept;

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
			/// \return Returns cursor position as a const reference to a glm::vec2.
			///
			[[nodiscard]] const glm::vec2& get_cursor_pos() noexcept;

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
			/// Map of galaxy mouse buttons to GLFW mouse buttons.
			///
			robin_hood::unordered_flat_map<input::MouseButton, int> m_mouse_map;

			///
			/// Map of GLFW mouse buttons to galaxy mouse buttons.
			///
			robin_hood::unordered_flat_map<int, input::MouseButton> m_reverse_mouse_map;

			///
			/// Map of galaxy keys to GLFW key codes.
			///
			robin_hood::unordered_flat_map<input::Keys, int> m_keymap;

			///
			/// Map of GLFW key codes to galaxy keys.
			///
			robin_hood::unordered_flat_map<int, input::Keys> m_reverse_keymap;

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
			/// Scene dispatcher.
			///
			events::Dispatcher* m_scene_dispatcher;

			///
			/// Cursor size.
			///
			glm::vec2 m_cursor_size;
		};
	} // namespace core
} // namespace galaxy

#endif