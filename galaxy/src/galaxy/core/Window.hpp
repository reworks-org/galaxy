///
/// Window.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_WINDOW_HPP_
#define GALAXY_CORE_WINDOW_HPP_

#include <entt/signal/dispatcher.hpp>

#include "galaxy/core/WindowSettings.hpp"
#include "galaxy/events/EventQueue.hpp"
#include "galaxy/graphics/PostProcess.hpp"
#include "galaxy/input/Clipboard.hpp"
#include "galaxy/input/Cursor.hpp"
#include "galaxy/input/Keyboard.hpp"

namespace galaxy
{
	namespace meta
	{
		///
		/// Checks if the input is an input device for a window.
		///
		/// \tparam T Parent type to test.
		///
		template<typename T>
		concept is_input_device = std::derived_from<T, input::InputDevice>;
	} // namespace meta

	namespace core
	{
		///
		/// Handles events, input & display.
		///
		class Window final
		{
		public:
			///
			/// Constructor.
			///
			Window() noexcept;

			///
			/// \brief Window creation constructor.
			///
			/// Calls open().
			///
			/// \param settings Settings of the window.
			///
			Window(const WindowSettings& settings);

			///
			/// Destructor.
			///
			~Window();

			///
			/// Construct a Window.
			///
			/// \param settings Settings of the window.
			///
			void open(const WindowSettings& settings);

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
			/// Destroys Window and related context and data.
			///
			void destroy();

			///
			/// Poll for events.
			///
			void poll_events() noexcept;

			///
			/// Prepare window for rendering.
			///
			void begin();

			///
			/// Finalize rendering.
			///
			void end();

			///
			/// Resizes window.
			///
			/// \param width Width of the window.
			/// \param height Height of the window.
			///
			void resize(const int width, const int height) noexcept;

			///
			/// Trigger all queued events for a dispatcher.
			///
			/// \param dispatcher An event dispatcher usually belonging to a scene.
			///
			void trigger_queued_events(entt::dispatcher& dispatcher);

			///
			/// \brief Notify's user of an event without interrupting.
			///
			/// Think like windows flashing tray icon.
			///
			void request_attention() noexcept;

			///
			/// Force window into focus.
			///
			void focus() const noexcept;

			///
			/// \brief Allow the window to be closed by the OS.
			///
			/// I.e from ALT+F4 or the close button in the window frame.
			///
			void allow_native_closing() noexcept;

			///
			/// \brief Prevent the window from being closed by the OS.
			///
			/// I.e from ALT+F4 or the close button in the window frame.
			///
			void prevent_native_closing() noexcept;

			///
			/// Set window title.
			///
			/// \param title New title to set window to.
			///
			void set_title(const char* title) noexcept;

			///
			/// Set window icon.
			///
			/// \param icon Icon to load.
			///
			void set_icon(std::string_view icon) noexcept;

			///
			/// Set window icon.
			///
			/// \param buffer Memory buffer containing pixels.
			///
			void set_icon(std::span<unsigned char> buffer) noexcept;

			///
			/// Check if windows is in focus.
			///
			/// \return True if window is in input focus.
			///
			[[nodiscard]] bool is_focused() const noexcept;

			///
			/// Get queued events.
			///
			/// \return Reference to EventQueue.
			///
			[[nodiscard]] events::Queue& queued_events() noexcept;

			///
			/// Get a window input device.
			///
			/// \tparam Device The type of input device to get. Constrained to a derived InputDevice.
			///
			/// \return Reference to the input device.
			///
			template<meta::is_input_device Device>
			[[nodiscard]] Device& get_input() noexcept;

			///
			/// \brief Get a list of paths dropped on window.
			///
			/// Reset when a new path is dropped.
			///
			[[nodiscard]] const std::vector<std::string>& get_drop_paths() const noexcept;

			///
			/// Get window width.
			///
			/// \return Integer.
			///
			[[nodiscard]] int get_width() const noexcept;

			///
			/// Get window height.
			///
			/// \return Integer.
			///
			[[nodiscard]] int get_height() const noexcept;

			///
			/// Retrieve pointer to GLFWwindow object.
			///
			/// \return Returns pointer to GLFWwindow.
			///
			[[nodiscard]] GLFWwindow* handle() noexcept;

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
			/// Width of window.
			///
			int m_width;

			///
			/// Height of window.
			///
			int m_height;

			///
			/// Post processor.
			///
			std::unique_ptr<graphics::PostProcess> m_postprocess;

			///
			/// Event queue.
			///
			events::Queue m_event_queue;

			///
			/// Keyboard assigned to window.
			///
			input::Keyboard m_keyboard;

			///
			/// Mouse assigned to window.
			///
			input::Mouse m_mouse;

			///
			/// Mouse cursor.
			///
			input::Cursor m_cursor;

			///
			/// Clipboard access.
			///
			input::Clipboard m_clipboard;

			///
			/// Cache of last dropped paths.
			///
			std::vector<std::string> m_drop_paths;
		};

		template<meta::is_input_device Device>
		inline Device& Window::get_input() noexcept
		{
			if constexpr (std::is_same<Device, input::Keyboard>::value)
			{
				return m_keyboard;
			}
			else if constexpr (std::is_same<Device, input::Mouse>::value)
			{
				return m_mouse;
			}
			else if constexpr (std::is_same<Device, input::Cursor>::value)
			{
				return m_cursor;
			}
			else if constexpr (std::is_same<Device, input::Clipboard>::value)
			{
				return m_clipboard;
			}
		}
	} // namespace core
} // namespace galaxy

#endif

/*
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
			/// Check if a mouse button was pressed.
			///
			/// \param mouse_button mouse button keycode.
			///
			/// \return True if a press occured.
			///
			[[nodiscard]] const bool mouse_button_pressed(input::MouseButtons mouse_button) noexcept;

			///
			/// Check if a mouse button was released.
			///
			/// \param mouse_button mouse button keycode.
			///
			/// \return True if a release occured.
			///
			[[nodiscard]] const bool mouse_button_released(input::MouseButtons mouse_button) noexcept;

			///
			/// Get mouse wheel delta.
			///
			/// \return Mouse scroll wheel delta.
			///
			[[nodiscard]] const double get_scroll_delta() noexcept;

			///
			/// Get current cursor position.
			///
			/// \return Returns cursor position as a glm::vec2.
			///
			[[nodiscard]] glm::vec2 get_cursor_pos() noexcept;

			///
			/// Get cursor size.
			///
			/// \return Const glm::vec2.
			///
			[[nodiscard]] const glm::vec2& cursor_size() const noexcept;

		private:

*/