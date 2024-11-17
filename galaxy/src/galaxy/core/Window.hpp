///
/// Window.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_WINDOW_HPP_
#define GALAXY_CORE_WINDOW_HPP_

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
			static void poll_events();

			///
			/// Set window focus.
			///
			static void focus();

			///
			/// Set window state: not minimized/maximized.
			///
			static void restore();

			///
			/// Set window state: minimized, if resizable.
			///
			static void minimize();

			///
			// Set window state: maximized, if resizable.
			///
			static void maximize();

			///
			/// Toggle window state: fullscreen/windowed.
			///
			static void toggle_fullscreen();

			///
			/// Toggle window state: borderless windowed.
			///
			static void toggle_borderless();

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
			static void set_pos(const int x, const int y);

			///
			/// Set window dimensions.
			///
			/// \param width Screen width of window.
			/// \param height Screen height of window.
			///
			static void set_size(const int width, const int height);

			///
			/// Should the window close?
			///
			/// \return True if window should close.
			///
			[[nodiscard]]
			static bool should_close();

			///
			/// Check if window is currently fullscreen.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			static bool is_fullscreen();

			///
			/// Check if window is currently hidden.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			static bool is_hidden();

			///
			/// Check if window is currently minimized.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			static bool is_minimized();

			///
			/// Check if window is currently maximized.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			static bool is_maximized();

			///
			/// Check if window is currently focused.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			static bool is_focused();

			///
			/// Get current screen width.
			///
			/// \return Integer.
			///
			[[nodiscard]]
			static int width();

			///
			/// Get current screen height.
			///
			/// \return Integer.
			///
			[[nodiscard]]
			static int height();

			///
			/// Get current render width (it considers HiDPI).
			///
			/// \return Integer.
			///
			[[nodiscard]]
			static int render_width();

			///
			/// Get current render height (it considers HiDPI).
			///
			/// \return Integer.
			///
			[[nodiscard]]
			static int render_height();

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
