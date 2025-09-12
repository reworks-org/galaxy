///
/// Window.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_WINDOW_HPP_
#define GALAXY_CORE_WINDOW_HPP_

#include <string>

#include <entt/signal/fwd.hpp>
#include <glm/vec2.hpp>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

#include "galaxy/input/Keyboard.hpp"
#include "galaxy/input/Mouse.hpp"

///
/// Forward declaration.
///
union SDL_Event;

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
		/// Handles all events for the window in this frame and sends to the dispatcher.
		///
		/// \param dispatcher Dispatcher to send event triggers to.
		///
		void process_events(entt::dispatcher& dispatcher);

		///
		/// \brief Close window.
		///
		/// This will exit game loop.
		///
		void close() noexcept;

		///
		/// Swap backbuffer with window framebuffer.
		///
		void swap() const noexcept;

		///
		/// Show window.
		///
		void show() const noexcept;

		///
		/// Hide window.
		///
		void hide() const noexcept;

		///
		/// Minimize window.
		///
		void minimize() const noexcept;

		///
		/// Maximize window.
		///
		void maximize() const noexcept;

		///
		/// Restore window to previous *mize state.
		///
		void restore() const noexcept;

		///
		/// Raise the window to be on top of other windows.
		///
		void raise() const noexcept;

		///
		/// Flash window on taskbar for user attention.
		///
		void request_attention() const noexcept;

		///
		/// Resizes window.
		///
		/// \param width Width of the window.
		/// \param height Height of the window.
		///
		void resize(const int width, const int height) const noexcept;

		///
		/// Toggle fullscreen.
		///
		/// \param fullscreen True for fullscreen mode, False for windowed mode.
		///
		void set_fullscreen(const bool fullscreen) const noexcept;

		///
		/// Sets the taskbar icon progress overlay.
		///
		/// \param progress Clamps from 0.0f (0%) to 1.0f (100%).
		///
		void set_taskbar_progress(const float progress) noexcept;

		///
		/// \brief Append to window title.
		///
		/// Does not preserve last append when called again.
		///
		/// \param append String to append.
		///
		void append_title(const std::string& append);

		///
		/// Set window icon.
		///
		/// \param icon Icon to load in VFS.
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
		/// Get mouse cursor.
		///
		/// \return Mouse ref.
		///
		[[nodiscard]]
		Mouse& get_mouse() noexcept;

		///
		/// Get keyboard.
		///
		/// \return Keyboard ref.
		///
		[[nodiscard]]
		Keyboard& get_keyboard() noexcept;

		///
		/// Get window size in pixels.
		///
		/// \return Integer vec2.
		///
		[[nodiscard]]
		glm::ivec2 get_pixel_size() noexcept;

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

		///
		/// Core event data.
		///
		SDL_Event m_events;

		///
		/// Keyboard assigned to window.
		///
		Keyboard m_keyboard;

		///
		/// Mouse assigned to window.
		///
		Mouse m_mouse;
	};
} // namespace galaxy

#endif
