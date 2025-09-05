///
/// Window.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#define SDL_STBIMG_SDL3

#include <glad/glad.h>
#include <SDL3/SDL_video.h>
#include <SDL_stbimage.h>

#include "galaxy/core/Settings.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/logging/Log.hpp"

#include "Window.hpp"

namespace galaxy
{
	Window::Window()
		: m_window {nullptr}
		, m_context {nullptr}
		, m_open {true}
	{
		if (SDL_ScreenSaverEnabled())
		{
			SDL_DisableScreenSaver();
		}

		SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_HIDDEN;
		if (settings::fullscreen())
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		else
		{
			if (settings::maximized())
			{
				flags |= SDL_WINDOW_MAXIMIZED;
			}
		}

		if (settings::mouse_grabbed())
		{
			flags |= SDL_WINDOW_MOUSE_GRABBED;
		}

		if (settings::window_resizable())
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		// SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, );      /**< the minimum number of bits for frame buffer size; defaults to 0. */
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		// SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, );   /**< the minimum number of bits for the red channel of the accumulation buffer; defaults to 0. */
		// SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, ); /**< the minimum number of bits for the green channel of the accumulation buffer; defaults to 0. */
		// SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, );  /**< the minimum number of bits for the blue channel of the accumulation buffer; defaults to 0. */
		// SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, ); /**< the minimum number of bits for the alpha channel of the accumulation buffer; defaults to 0. */
		SDL_GL_SetAttribute(SDL_GL_STEREO, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG | SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 0);
		SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);
		// SDL_GL_SetAttribute(SDL_GL_CONTEXT_RELEASE_BEHAVIOR, );   /**< sets context the release behavior. See SDL_GLContextReleaseFlag; defaults to FLUSH. */
		// SDL_GL_SetAttribute(SDL_GL_CONTEXT_RESET_NOTIFICATION, ); /**< set context reset notification. See SDL_GLContextResetNotification; defaults to NO_NOTIFICATION. */

		m_window = SDL_CreateWindow(settings::title().c_str(), settings::window_width(), settings::window_height(), flags);
		if (m_window)
		{
			m_context = SDL_GL_CreateContext(m_window);
			if (m_context)
			{
				if (SDL_GL_MakeCurrent(m_window, m_context))
				{
					if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
					{
						SDL_GL_SetSwapInterval(settings::vsync());
						SDL_SetWindowAlwaysOnTop(m_window, false);
						SDL_SetWindowBordered(m_window, settings::window_border());
						SDL_SetWindowFocusable(m_window, true);
						SDL_SetWindowFullscreenMode(m_window, nullptr); // Borderless Fullscreen mode.
						SDL_SetWindowKeyboardGrab(m_window, false);
						SDL_SetWindowMinimumSize(m_window, 640, 360);
						SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

																		// ...
					}
					else
					{
						GALAXY_LOG(GALAXY_FATAL, "Failed to initialize glad.");
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to initialize OpenGL context into SDL.");
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to initialize OpenGL context.");
			}
		}
		else
		{
			GALAXY_LOG(GALAXY_FATAL, "Failed to initialize SDL window.");
		}
	}

	Window::~Window()
	{
		SDL_GL_DestroyContext(m_context);
		SDL_DestroyWindow(m_window);

		m_context = nullptr;
		m_window  = nullptr;
		m_open    = false;
	}

	void Window::maximize() const noexcept
	{
		SDL_MaximizeWindow(m_window);
		SDL_SyncWindow(m_window);
		// TODO: trigger resize.
	}

	void Window::minimize() const noexcept
	{
		SDL_MinimizeWindow(m_window);
		SDL_SyncWindow(m_window);
	}

	void Window::restore() const noexcept
	{
		SDL_RestoreWindow(m_window);
		SDL_SyncWindow(m_window);
		// TODO: trigger resize.
	}

	void Window::show() const noexcept
	{
		SDL_ShowWindow(m_window);
	}

	void Window::hide() const noexcept
	{
		SDL_HideWindow(m_window);
	}

	void Window::raise() const noexcept
	{
		SDL_RaiseWindow(m_window);
	}

	void Window::request_attention() const noexcept
	{
		SDL_FlashWindow(m_window, SDL_FlashOperation::SDL_FLASH_UNTIL_FOCUSED);
	}

	void Window::swap() const noexcept
	{
		SDL_GL_SwapWindow(m_window);
	}

	void Window::close() noexcept
	{
		m_open = false;
	}

	void Window::resize(const int width, const int height) const noexcept
	{
		SDL_SetWindowSize(m_window, width, height);
		// TODO: trigger resize.
	}

	void Window::append_title(const std::string& append)
	{
		const auto title = settings::title() + append;
		SDL_SetWindowTitle(m_window, title.c_str());
	}

	void Window::set_fullscreen(const bool fullscreen) const noexcept
	{
		SDL_SetWindowFullscreen(m_window, fullscreen);
		// TODO: trigger resize.
	}

	void Window::set_mouse_grab(const bool grabbed) const noexcept
	{
		SDL_SetWindowMouseGrab(m_window, grabbed);
	}

	void Window::set_taskbar_progress(const float progress) noexcept
	{
		SDL_SetWindowProgressValue(m_window, std::clamp(progress, 0.0f, 1.0f));
	}

	void Window::set_icon(const std::string& icon) noexcept
	{
		if (!icon.empty())
		{
			auto& fs = entt::locator<VirtualFileSystem>::value();

			auto data = fs.read_binary(icon);
			if (!data.empty())
			{
				// stbi_set_flip_vertically_on_load(true);

				auto surface = STBIMG_LoadFromMemory(data.data(), static_cast<int>(data.size()));
				if (surface)
				{
					SDL_SetWindowIcon(m_window, surface);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, SDL_GetError());
				}

				SDL_DestroySurface(surface);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}' from the vfs.", icon);
			}
		}
	}

	bool Window::is_open() const noexcept
	{
		return m_open;
	}

	SDL_Window* Window::handle() const noexcept
	{
		return m_window;
	}

	SDL_GLContext Window::context() const noexcept
	{
		return m_context;
	}
} // namespace galaxy
