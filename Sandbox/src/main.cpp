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
#include <qs/utils/Error.hpp>
#include <qs/core/Shader.hpp>
#include <qs/vertex/VertexArray.hpp>

int main(int argsc, char* argsv[])
{
	// Try to init SDL2 with passed flags, checking to make sure of success.
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Init failed. SDL_Error: " << SDL_GetError() << std::endl;
	}
	else
	{
		qs::Error::handle.setQSCallback([](std::string_view file, unsigned int line, std::string_view message) -> void
		{
			std::cout << "[Quasar Error] File: " << file << " Line: " << line << " Message: " << message << std::endl;
		});

		// Create window and check for errors.
		qs::Window window;
		if (!window.create("TestBed", 800, 600, SDL_WINDOW_ALLOW_HIGHDPI))
		{
			std::cout << "Window creation failed!" << std::endl;
		}

		qs::Error::handle.setGLCallback([](unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) -> void
		{
			std::cout << "[GL_ERROR] Source: " << source << " Type: " << type << " ID: " << id << " Severity: " << severity << " Length: " << length << " Message: " << message << std::endl;
		});

		// Events
		SDL_Event e;

		// Shaders
		qs::Shader shader("bin/basic.vert", "bin/basic.frag");
		
		// Triangle verticies
		// x, y, r, g, b
		std::array<float, 18> vertices =
		{
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
		};

		std::array<unsigned int, 3> indices =
		{
			0, 1, 2
		};

		qs::VertexBuffer vb;
		vb.create(vertices, GL_STATIC_DRAW);

		qs::IndexBuffer ib;
		ib.create(indices, GL_STATIC_DRAW);

		qs::VertexLayout layout;
		layout.add<float>(3); // pos
		layout.add<float>(3); // col

		qs::VertexArray va(vb, ib, layout);

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
				window.onSizeChanged(e.window.data1, e.window.data2);
				break;
			}

			// Render.
			window.clear(qs::Colour{ 1.0f, 1.0f, 1.0f, 1.0f });

			shader.use();
			va.bind();

			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
			
			window.swap();
		}

		// Exit.
		window.destroy();
		SDL_Quit();
	}

	return 0;
}