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
#include <qs/renderer/Renderer.hpp>
#include <qs/transforms/Camera.hpp>
#include <qs/graphics/Sprite2D.hpp>
#include <qs/core/TextureAtlas.hpp>
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

		float aspectRatio = 16.0f / 9.0f;
		if (!window.create("TestBed", 1024, 768, settings))
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
		
		qs::Sprite2D sprite;
		sprite.load("bin/wall.png");
		sprite.create();
		sprite.move(50.0f, 50.0f);
		sprite.rotate(45.0f);
		//sprite.scale(200, 200);
		
		//qs::Shader rttshader;
		//rttshader.load(std::filesystem::path{ "bin/rtt.vert" }, std::filesystem::path{ "bin/rtt.frag" });
		//qs::TextureAtlas atlas(640);
		//atlas.add({ "bin/arrow.png" });
		//atlas.create(rttshader);
		
		qs::Camera camera; //left, right, bottom, top
		camera.configure(0.0f, window.getWidth(),window.getHeight(), 0.0f);
		//camera.setSpeed(0.2f);
		//camera.scale(.0f, 8.0f);

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

			case SDL_WINDOWEVENT_SIZE_CHANGED:
				window.resize(e.window.data1, e.window.data2);
				break;
			}

			camera.update(1.0);
			shader.use();
			shader.setUniform<glm::mat4>("u_camera", camera.get());

			// Render.
			window.begin(qs::Colours::White);
			
			renderer.drawSprite2D(sprite, shader);
			//renderer.drawSprite2D(atlas.getAtlas(), shader);

			window.end();
		}

		// Exit.
		window.destroy();
		SDL_Quit();
	}

	return 0;
}