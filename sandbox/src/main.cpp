///
/// main.cpp
/// Sandbox
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <Windows.h>
#undef DELETE // fucking windows

#include <SDL2/SDL.h>

#include <protostar/system/Keys.hpp>
#include <qs/utils/Error.hpp>
#include <qs/core/Texture.hpp>
#include <qs/core/Window.hpp>
#include <qs/render/Renderer.hpp>

int main(int argsc, char* argsv[])
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 14);
	
	// Try to init SDL2 with passed flags, checking to make sure of success.
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Init failed. SDL_Error: " << SDL_GetError() << std::endl;
	}
	else
	{
		qs::Error::handle().setQSCallback([](std::string_view file, unsigned int line, std::string_view message) -> void
		{
			std::cout << "[Quasar Error] File: " << file << " Line: " << line << " Message: " << message << std::endl;
		});

		// Create window and check for errors.
		qs::Window window;
		if (!window.create("TestBed", 800, 600, SDL_WINDOW_ALLOW_HIGHDPI))
		{
			std::cout << "Window creation failed!" << std::endl;
		}

		qs::Error::handle().setGLCallback([](unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) -> void
		{
			std::cout << "[GL_ERROR] Source: " << source << " Type: " << type << " ID: " << id << " Severity: " << severity << " Length: " << length << " Message: " << message << std::endl;
		});

		// Events
		SDL_Event e;

		// Shaders
		qs::Shader shader("bin/basic.vert", "bin/basic.frag");
		
		// rect verticies
		// x, y, z, r, g, b, tex, tex
		std::array<float, 32> vertices =
		{
			0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
		};

		std::array<float, 32> new_vertices =
		{
			-50.0f, -50.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
			50.0f, -50.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
			50.0f, 50.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
			-50.0f,  50.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
		};

		std::array<unsigned int, 6> indices =
		{
			0, 1, 3,
			1, 2, 3
		};

		qs::VertexBuffer vb;
		vb.create(vertices);

		qs::IndexBuffer ib;
		ib.create(indices);

		qs::VertexLayout layout(8);
		layout.add<float>(3); // pos
		layout.add<float>(3); // col
		layout.add<float>(2); // tex

		qs::VertexArray va(vb, ib, layout);

		qs::Texture tex("bin/wall.jpg");

		qs::Camera camera;
		camera.configure(-2.0f, 2.0f, -2.0f, 2.0f);
		camera.setSpeed(0.2f);

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

				case SDLK_UP:
					camera.onKeyDown({ protostar::Keys::UP });
					break;

				case SDLK_DOWN:
					camera.onKeyDown({ protostar::Keys::DOWN });
					break;

				case SDLK_LEFT:
					camera.onKeyDown({ protostar::Keys::LEFT });
					break;

				case SDLK_RIGHT:
					camera.onKeyDown({ protostar::Keys::RIGHT });
					break;

				case SDLK_q:
					camera.onKeyDown({ protostar::Keys::Q });
					break;

				case SDLK_e:
					camera.onKeyDown({ protostar::Keys::E });
					break;
				}
				break;

			case SDL_WINDOWEVENT_SIZE_CHANGED:
				window.onSizeChanged(e.window.data1, e.window.data2);
				break;
			}

			camera.update(1.0);
			shader.use();
			shader.setUniform<glm::mat4>("u_proj", camera.get());

			// Render.
			window.clear(qs::Colours::White);

			tex.bind();
			va.bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			
			window.swap();
		}

		// Exit.
		window.destroy();
		SDL_Quit();
	}

	return 0;
}