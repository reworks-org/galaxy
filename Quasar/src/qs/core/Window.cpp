///
/// Window.cpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "qs/libs/glad/glad.h"

#include "Window.hpp"

namespace qs
{
	Window::Window() noexcept
		:m_isOpen(true), m_window(nullptr), m_glContext(nullptr)
	{
	}

	Window::~Window() noexcept
	{
		// Call again to ensure everything is cleaned up.
		// Has checks to ensure no null data is destroyed.
		destroy();
	}

	qs::Result Window::create(const std::string& title, int w, int h, Uint32 windowFlags) noexcept
	{
		// Function result.
		qs::Result result;

		// Set the version of OpenGL we want to use.
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// Hardware acceleration
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		// Double buffering.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		// Make sure forward-compatible context.
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

		// Create the window from input, ensuring it is centered in the screen.
		m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, windowFlags | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		// Then if the window failed to create:
		if (!m_window)
		{
			// Set error info.
			result.m_message = "Window failed to create! SDL_Error: ";
			result.m_message += SDL_GetError();
			result.m_status = qs::Result::Status::FAILURE;
		}
		else
		{
			// Create GL context. This also makes context current. No need to call SDL_GL_MakeCurrent().
			m_glContext = SDL_GL_CreateContext(m_window);

			// Then if context failed to create:
			if (!m_glContext)
			{
				// Set error info.
				result.m_message = "OpenGL context failed to be created! SDL_Error: ";
				result.m_message += SDL_GetError();
				result.m_status = qs::Result::Status::FAILURE;
			}
			else
			{
				// Set up glad gl loader with SDL2.
				if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
				{
					result.m_message = "Failed to init glad. glError: ";
					result.m_message += std::to_string(glad_glGetError());
					result.m_status = qs::Result::Status::FAILURE;
				}
				else
				{
					// Set up the viewport.
					glViewport(0, 0, w, h);
				}
			}
		}

		return result;
	}

	void Window::destroy() noexcept
	{
		// Clean up GL context, checking to make sure its not already been destroyed.
		if (m_glContext != nullptr)
		{
			SDL_GL_DeleteContext(m_glContext);
			m_glContext = nullptr;
		}

		// Clean up window data, checking to make sure its not already been destroyed.
		if (m_window != nullptr)
		{
			SDL_DestroyWindow(m_window);
			m_window = nullptr;
		}
	}

	bool Window::isOpen() const noexcept
	{
		return m_isOpen;
	}

	void Window::close() noexcept
	{
		m_isOpen = false;
	}

	void Window::onSizeChanged(int w, int h) noexcept
	{
		glViewport(0, 0, w, h);
	}

	void Window::resize(int w, int h) noexcept
	{
		SDL_SetWindowSize(m_window, w, h);
		glViewport(0, 0, w, h);
	}

	void Window::clear(const qs::Colour& colour) noexcept
	{
		glClearColor(colour.m_red, colour.m_green, colour.m_blue, colour.m_alpha);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::swap() noexcept
	{
		SDL_GL_SwapWindow(m_window);
	}

	SDL_Window* Window::getWindow() noexcept
	{
		return m_window;
	}

	SDL_GLContext& Window::getContext() noexcept
	{
		return m_glContext;
	}
}