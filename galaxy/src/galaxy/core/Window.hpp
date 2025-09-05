///
/// Window.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_WINDOW_HPP_
#define GALAXY_CORE_WINDOW_HPP_

#include <SDL3/SDL_video.h>

namespace galaxy
{
	///
	/// RAII Window.
	/// Handles events, input & display.
	///
	class Window final
	{
	public:
		///
		/// Window creation constructor.
		///
		Window();

		///
		/// Destructor.
		///
		~Window();

		///
		/// Maximize window.
		///
		void maximize() const noexcept;

		///
		/// Minimize window.
		///
		void minimize() const noexcept;

		///
		/// Restore window.
		///
		void restore() const noexcept;

		///
		/// Show window.
		///
		void show() const noexcept;

		///
		/// Hide window.
		///
		void hide() const noexcept;

		///
		/// Raise the window to be on top of other windows.
		///
		void raise() const noexcept;

		///
		/// Flash window on taskbar for user attention.
		///
		void request_attention() const noexcept;

		///
		/// Swap backbuffer with window framebuffer.
		///
		void swap() const noexcept;

		///
		/// \brief Close window.
		///
		/// This will exit game loop.
		///
		void close() noexcept;

		///
		/// Resizes window.
		///
		/// \param width Width of the window.
		/// \param height Height of the window.
		///
		void resize(const int width, const int height) const noexcept;

		///
		/// \brief Append to window title.
		///
		/// Does not preserve last append when called again.
		///
		/// \param append String to append.
		///
		void append_title(const std::string& append);

		///
		/// Toggle fullscreen.
		///
		/// \param fullscreen True for fullscreen mode, False for windowed mode.
		///
		void set_fullscreen(const bool fullscreen) const noexcept;

		///
		/// Toggle mouse grab.
		///
		/// \param grabbed This is true to grab keyboard, and false to release.
		///
		void set_mouse_grab(const bool grabbed) const noexcept;

		///
		/// Sets the taskbar icon progress overlay.
		///
		/// \param progress Clamps from 0.0f (0%) to 1.0f (100%).
		///
		void set_taskbar_progress(const float progress) noexcept;

		///
		/// Set window icon.
		///
		/// \param icon Icon to load.
		///
		void set_icon(const std::string& icon) noexcept;

		///
		/// Is the window open or closed.
		///
		/// \return Boolean.
		///
		[[nodiscard]]
		bool is_open() const noexcept;

		///
		/// Get SDL window pointer.
		///
		/// \return Raw sdl window pointer.
		///
		[[nodiscard]]
		SDL_Window* handle() const noexcept;

		///
		/// Get SDL GL context.
		///
		/// \return Opaque handle (pointer).
		///
		[[nodiscard]]
		SDL_GLContext context() const noexcept;

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
		/// SDL window handle.
		///
		SDL_Window* m_window;

		///
		/// SDL OpenGL context.
		///
		SDL_GLContext m_context;

		///
		/// Window state flag.
		///
		bool m_open;
	};
} // namespace galaxy

#endif
