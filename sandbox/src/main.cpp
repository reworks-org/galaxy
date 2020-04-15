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
#include <qs/core/WindowSettings.hpp>
#include <qs/core/Texture.hpp>
#include <qs/core/Window.hpp>
#include <qs/renderer/Renderer.hpp>
#include <qs/graphics/Camera.hpp>
#include <qs/graphics/Sprite.hpp>
#include <qs/core/Shader.hpp>
#include <qs/core/RenderTexture.hpp>
#include <qs/graphics/TextureAtlas.hpp>

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
		// Have to define window settings before creating window.
		qs::WindowSettings::s_hardwareRendering = true;
		qs::WindowSettings::s_msaa = true;
		qs::WindowSettings::s_msaaLevel = 2;
		qs::WindowSettings::s_windowFlags = SDL_WINDOW_ALLOW_HIGHDPI;

		qs::Window window;

		if (!window.create("TestBed", 1024, 768))
		{
			std::cout << "Window creation failed!" << std::endl;
		}

		qs::Error::handle().setGLCallback([](unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) -> void
		{
			std::cout << "[GL_MSG]: Severity: " << severity << " Message: " << message << std::endl;
		});

		SDL_Event e;
		qs::Renderer renderer;

		// Shaders
		qs::Shader shader(std::filesystem::path("bin/sprite.vert"), std::filesystem::path("bin/sprite.frag"));
		qs::Shader rttshader(std::filesystem::path("bin/rtt.vert"), std::filesystem::path("bin/rtt.frag"));

		// Texture atlas is allowed to bind/unbind shaders - the only one allowed.
		qs::TextureAtlas atlas;
		atlas.add("bin/wall_2.png");
		atlas.create(window, renderer, rttshader);
		atlas.save("bin/atlas");

		qs::Sprite wall;
		wall.load("bin/wall.png");
		wall.create(qs::BufferType::DYNAMIC);
		wall.move(0.0f, 0.0f);

		auto atlasSpr = atlas.getSprite();

		qs::Camera camera; //left, right, bottom, top
		camera.create(0.0f, window.getWidth(), window.getHeight(), 0.0f);
		camera.setSpeed(0.2f);

		shader.bind();

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
			shader.setUniform<glm::mat4>("u_cameraProj", camera.getProj());
			shader.setUniform<glm::mat4>("u_cameraView", camera.getTransformation());

			// Render.
			window.begin(qs::Colours::White);
			
			renderer.drawSprite(atlasSpr, shader);
			//renderer.drawSprite(wall, shader);

			window.end();
		}

		// Exit.
		window.destroy();
		SDL_Quit();
	}

	return 0;
}