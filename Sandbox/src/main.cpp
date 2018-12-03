///
/// main.cpp
/// Sandbox
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <SDL2/SDL.h>
#include <qs/core/Window.hpp>

int main(int argsc, char* argsv[])
{
	// Try to init SDL2 with passed flags, checking to make sure of success.
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Init failed. SDL_Error: " << SDL_GetError() << std::endl;
	}
	else
	{
		// Create window and check for errors.
		qs::Window window;
		qs::Result result = window.create("TestBed", 800, 600, SDL_WINDOW_ALLOW_HIGHDPI);
		if (!result)
		{
			std::cout << "Window failed. Error: " << result.m_message;
		}

		// Events
		SDL_Event e;

		// Loop
		while (window.isOpen())
		{
			SDL_PollEvent(&e);

			// Handle resizing and quitting events.
			switch (e.type)
			{
			case SDL_QUIT:
				window.close();
				break;

			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					window.close();
					break;
				}
				break;

			case SDL_WINDOWEVENT_SIZE_CHANGED:
				window.resize(e.window.data1, e.window.data2);
				break;
			}

			// Render.
			window.clear(qs::Colour{ 0.0f, 0.0f, 0.2f, 1.0f });
			window.swap();
		}

		// Exit.
		window.destroy();
		SDL_Quit();
	}

	return 0;
}