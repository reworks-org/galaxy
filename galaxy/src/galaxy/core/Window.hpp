///
/// Window.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_WINDOW_HPP_
#define GALAXY_CORE_WINDOW_HPP_

#include <entt/signal/fwd.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "galaxy/core/WindowSettings.hpp"
#include "galaxy/input/Clipboard.hpp"
#include "galaxy/input/Cursor.hpp"
#include "galaxy/input/Keyboard.hpp"
#include "galaxy/input/Mouse.hpp"

namespace galaxy
{
	namespace core
	{
		///
		/// Handles events, input & display.
		///
		class Window final
		{
		  public:
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
			/// Destroys Window and related context and data.
			///
			void destroy();

			///
			/// Poll for events.
			///
			void poll_events();

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
			void request_attention();

			///
			/// Force window into focus.
			///
			void focus() const;

			///
			/// Maximize window.
			///
			void maximize();

			///
			/// Restore window.
			///
			void restore();

			///
			/// Minimize window.
			///
			void minimize() const;

			///
			/// Set window into borderless fullscreen mode.
			///
			void fullscreen();

			///
			/// Set window title.
			///
			/// \param title New title to set window to.
			///
			void set_title(const std::string& title);

			///
			/// Append to window title.
			///
			/// \param append Appends to existing title, does not preserve last append when called again.
			///
			void append_title(const std::string& append);

			///
			/// Set window icon.
			///
			/// \param icon Icon to load.
			///
			void set_icon(const std::string& icon);

			///
			/// Set window icon.
			///
			/// \param buffer Memory buffer containing pixels.
			///
			void set_icon(std::span<std::uint8_t> buffer);

			///
			/// Set current event dispatcher to use.
			///
			/// \param dispatcher Event dispatcher to use for processing input, etc.
			///
			void set_dispatcher(entt::dispatcher* dispatcher);

			///
			/// Check if windows is in focus.
			///
			/// \return True if window is in input focus.
			///
			[[nodiscard]] bool is_focused() const;

			[[nodiscard]] int   window_width() const;
			[[nodiscard]] int   window_height() const;
			[[nodiscard]] int   frame_width() const;
			[[nodiscard]] int   frame_height() const;
			[[nodiscard]] float aspect_ratio() const;

			///
			/// Get a window input device.
			///
			/// \tparam Device The type of input device to get. Constrained to a derived InputDevice.
			///
			/// \return Reference to the input device.
			///
			template<meta::is_input_device Device>
			[[nodiscard]] Device& get_input();

			///
			/// \brief Get a list of paths dropped on window.
			///
			/// Reset when a new path is dropped.
			///
			[[nodiscard]] const meta::vector<std::string>& get_drop_paths() const;

			///
			/// Get framebuffer size taking into account DPI.
			///
			/// \return glm::ivec2.
			///
			[[nodiscard]] glm::ivec2 get_framebuffer_size();

			///
			/// Get window content scale.
			///
			/// \return glm::vec2.
			///
			[[nodiscard]] glm::vec2 get_content_scale();

			///
			/// Get max window content scale.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_content_scale_max();

			///
			/// Retrieve pointer to GLFWwindow object.
			///
			/// \return Returns pointer to GLFWwindow.
			///
			[[nodiscard]] GLFWwindow* handle();

		  private:
			///
			/// Constructor.
			///
			Window() = delete;

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
			/// Window title.
			///
			std::string m_title;

			///
			/// Width of window (or fullscreen).
			///
			int m_window_width;

			///
			/// Height of window (or fullscreen).
			///
			int m_window_height;

			///
			/// The virtual or final framebuffer width.
			///
			int m_frame_width;

			///
			/// The virtual or final framebuffer height.
			///
			int m_frame_height;

			///
			/// Window aspect ratio.
			///
			float m_aspect_ratio;

			///
			/// GLFW window data.
			///
			GLFWwindow* m_window;

			///
			/// Currently active event dispatcher.
			///
			entt::dispatcher* m_dispatcher;

			///
			/// Custom GLFW window allocator.
			///
			GLFWallocator m_glfw_allocator;

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
			meta::vector<std::string> m_drop_paths;
		};

		template<meta::is_input_device Device>
		inline Device& Window::get_input()
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
