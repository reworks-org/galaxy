///
/// Window.cpp
/// Quasar
///
/// Created by reworks on 16/09/2018.
/// MIT LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <stdexcept>

#include "Window.hpp"

namespace qs
{
	Window::Window(const std::string& title, int w, int h, Uint32 windowFlags, Uint32 renderFlags)
		:m_isOpen(true), m_window(nullptr), m_renderer(nullptr)
	{
		// Create the window from input, ensuring it is centered in the screen.
		m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, windowFlags);

		// Then if the window failed to create:
		if (!m_window)
		{
			// Get the SDL error message.
			std::string message = "Window failed to be created! SDL: ";
			message += SDL_GetError();

			// And throw an exception with information about the error.
			throw std::runtime_error(message);
		}

		// Create the renderer from the window and inputted flags.
		m_renderer = SDL_CreateRenderer(m_window, -1, renderFlags);

		// Then if renderer failed to create:
		if (!m_renderer)
		{
			// Get the SDL error message.
			std::string message = "Renderer failed to be created! SDL: ";
			message += SDL_GetError();

			// Then throw exception.
			throw std::runtime_error(message);
		}
	}

	Window::~Window()
	{
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
	}

	SDL_Renderer* Window::getRenderer() const noexcept
	{
		return m_renderer;
	}
}