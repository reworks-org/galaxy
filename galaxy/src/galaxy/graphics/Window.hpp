///
/// Window.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_WINDOW_HPP_
#define GALAXY_GRAPHICS_WINDOW_HPP_

#include <GLFW/glfw3.h>

#include "galaxy/graphics/WindowSettings.hpp"

namespace galaxy
{
	namespace graphics
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
			[[nodiscard]] bool open(const WindowSettings& settings);

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
			/// Set window title.
			///
			/// \param title New title to set window to.
			///
			void set_title(const char* title);

			///
			/// Check if windows is in focus.
			///
			/// \return True if window is in input focus.
			///
			[[nodiscard]] bool is_focused() noexcept;

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
		};
	} // namespace graphics
} // namespace galaxy

#endif