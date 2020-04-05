///
/// main.cpp
/// Sandbox
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <SDL2/SDL.h>

#include <protostar/system/Keys.hpp>
#include <qs/utils/Error.hpp>
#include <qs/core/Texture.hpp>
#include <qs/core/Window.hpp>
#include <qs/render/Renderer.hpp>
#include <qs/transforms/Camera.hpp>
#include <qs/render/Sprite2D.hpp>
#include <qs/core/Shader.hpp>

int main(int argsc, char* argsv[])
{
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
		qs::Window::WindowSettings settings;
		settings.hardwareRendering = true;
		settings.msaa = true;
		settings.msaaLevel = 2;
		settings.SDL_windowFlags = SDL_WINDOW_ALLOW_HIGHDPI;

		if (!window.create("TestBed", 800, 600, settings))
		{
			std::cout << "Window creation failed!" << std::endl;
		}

		qs::Error::handle().setGLCallback([](unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) -> void
		{
			std::cout << "[GL_MSG]: Severity: " << severity << " Message: " << message << std::endl;
		});

		// Events
		SDL_Event e;

		// Shaders
		qs::Shader shader(std::filesystem::path("bin/basic.vert"), std::filesystem::path("bin/basic.frag"));
		
		// rect verticies
		// x, y, z, r, g, b, a, tex, tex
		std::array<float, 36> old_vertices =
		{
			50.5f, 50.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,   // top right
			50.5f, -50.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f,   // bottom right
			-50.5f, -50.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f,   // bottom left
			-50.5f,  50.5f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f    // top left 
		};

		// x, y, z, tex, tex
		qs::Sprite2D sprite;
		sprite.addVertexes<20>({
			50.5f, 50.5f, 0.0f, 1.0f, 1.0f,   // top right
			50.5f, -50.5f, 0.0f, 1.0f, 0.0f,   // bottom right
			-50.5f, -50.5f, 0.0f, 0.0f, 0.0f,   // bottom left
			-50.5f,  50.5f, 0.0f, 0.0f, 1.0f    // top left 
			});

		sprite.addIndexes<6>({
			0, 1, 3,
			1, 2, 3
			});

		// pos, tex
		sprite.addLayout(5, { 3, 2 });
		sprite.create();
		sprite.load("bin/wall.png");
		sprite.rotate(45.0f);
		
		qs::Camera camera;
		//camera.configure(-2.0f, 2.0f, -2.0f, 2.0f);
		camera.configure(0.0f, window.getWidth(), window.getHeight(), 0.0f);
		camera.setSpeed(0.2f);
		//camera.scale(8.0f, 8.0f);

		qs::Renderer renderer;

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
			}

			camera.update(1.0);
			shader.use();
			shader.setUniform<glm::mat4>("u_camera", camera.get());

			// Render.
			window.begin(qs::Colours::White);
			
			renderer.drawSprite2D(sprite, shader);

			window.end();
		}

		// Exit.
		window.destroy();
		SDL_Quit();
	}

	return 0;
}