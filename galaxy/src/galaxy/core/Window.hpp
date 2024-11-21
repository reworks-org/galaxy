///
/// Window.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_WINDOW_HPP_
#define GALAXY_CORE_WINDOW_HPP_

#include <string>

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
			/// Constructor.
			///
			Window();

			///
			/// Destructor.
			///
			~Window();

			///
			/// Poll for events.
			///
			void poll_events();

			///
			/// Swap draw buffers.
			///
			void swap_buffers();

			///
			/// Set window focus.
			///
			void focus();

			///
			/// Set window state: not minimized/maximized.
			///
			void restore();

			///
			/// Set window state: minimized, if resizable.
			///
			void minimize();

			///
			// Set window state: maximized, if resizable.
			///
			void maximize();

			///
			/// Toggle window state: fullscreen/windowed.
			///
			void toggle_fullscreen();

			///
			/// Toggle window state: borderless windowed.
			///
			void toggle_borderless();

			///
			/// Set icon for window.
			///
			/// \param icon Icon file in VFS.
			///
			void set_icon(const std::string& icon);

			///
			/// Set title for window.
			///
			/// \param title New window bar title.
			///
			void set_title(const std::string& title);

			///
			/// Append to window title.
			///
			/// \param append Appends to existing title, does not preserve last append when called again.
			///
			void append_title(const std::string& append);

			///
			/// Set window position on screen.
			///
			/// \param x x screen coord.
			/// \param y y screen coord.
			///
			void set_pos(const int x, const int y);

			///
			/// Set window dimensions.
			///
			/// \param width Screen width of window.
			/// \param height Screen height of window.
			///
			void set_size(const int width, const int height);

			///
			/// Should the window close?
			///
			/// \return True if window should close.
			///
			[[nodiscard]]
			bool should_close() const;

			///
			/// Check if window is currently fullscreen.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_fullscreen() const;

			///
			/// Check if window is currently hidden.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_hidden() const;

			///
			/// Check if window is currently minimized.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_minimized() const;

			///
			/// Check if window is currently maximized.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_maximized() const;

			///
			/// Check if window is currently focused.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_focused() const;

			///
			/// \brief Get a list of paths dropped on window.
			///
			/// Reset when new path(s) are dropped.
			///
			/// \return List of paths as a string.
			///
			[[nodiscard]]
			std::vector<std::string> get_drop_paths() const;

			///
			/// Get current screen width.
			///
			/// \return Integer.
			///
			[[nodiscard]]
			int width() const;

			///
			/// Get current screen height.
			///
			/// \return Integer.
			///
			[[nodiscard]]
			int height() const;

			///
			/// Get current render width (it considers HiDPI).
			///
			/// \return Integer.
			///
			[[nodiscard]]
			int render_width() const;

			///
			/// Get current render height (it considers HiDPI).
			///
			/// \return Integer.
			///
			[[nodiscard]]
			int render_height() const;

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
			/// Title cache.
			///
			std::string m_title;
		};
	} // namespace core
} // namespace galaxy

#endif
