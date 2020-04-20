///
/// main.cpp
/// Sandbox
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

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
#include <qs/text/Text.hpp>

int main(int argsc, char* argsv[])
{
	qs::Error::handle().setQSCallback([](std::string_view file, unsigned int line, std::string_view message) -> void
		{
			std::cout << "[Quasar Error] File: " << file << " Line: " << line << " Message: " << message << std::endl;
		});

	qs::WindowSettings::s_antiAliasing = 2;
	qs::WindowSettings::s_ansiotropicFiltering = 2;
	qs::WindowSettings::s_vsync = false;
	qs::WindowSettings::s_srgb = false;
	qs::WindowSettings::s_aspectRatioX = -1;
	qs::WindowSettings::s_aspectRatioY = -1;
	qs::WindowSettings::s_rawMouseInput = true;

	qs::Window window;

	if (!window.create("TestBed", 1024, 768))
	{
		std::cout << "Window creation failed!" << std::endl;
	}

	qs::Error::handle().setGLCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) -> void
		{
			std::cout << "[GL_MSG]: Severity: " << severity << " Message: " << message << std::endl;
		});

	qs::Renderer renderer;

	// Shaders
	qs::Shader shader;
	shader.loadFromPath("bin/sprite.vert", "bin/sprite.frag");

	qs::Shader rttshader;
	rttshader.loadFromPath("bin/rtt.vert", "bin/rtt.frag");

	qs::Shader textRTTshader;
	textRTTshader.loadFromPath("../quasar/res/shaders/renderTextToTexture.glsl");

	// Texture atlas is allowed to bind/unbind shaders - the only one allowed.
	qs::TextureAtlas atlas;
	atlas.add("bin/wall.png");
	atlas.add("bin/wall_2.png");
	atlas.create(window, renderer, rttshader);
	atlas.save("bin/atlas");

	//qs::Sprite wall;
	//wall.load("bin/wall.png");
	//wall.create(qs::BufferType::DYNAMIC);
	//wall.move(250.0f, 250.0f);
	//wall.rotate(45.0f);

	auto atlasSpr = atlas.getSprite();
	atlasSpr.setActiveQuad(atlas.getID("wall"));
	atlasSpr.move(0.0f, 0.0f);

	atlasSpr.setActiveQuad(atlas.getID("wall_2"));
	atlasSpr.move(500.0f, 500.0f);
	
	atlasSpr.applyTransforms();

	qs::Text text;
	qs::Font font;
	font.create("bin/public.ttf", 36);
	auto col = qs::Colours::Black;
	text.load("HELLO, WORLD.", font, col);
	textRTTshader.bind();
	text.create(window, renderer, textRTTshader);
	text.asSprite().save("bin/text");

	qs::Camera camera; //left, right, bottom, top
	camera.create(0.0f, window.getWidth(), window.getHeight(), 0.0f);
	camera.setSpeed(0.2f);

	shader.bind();

	//atlasSpr.setActiveQuad(atlas.getID("wall"));

	// Loop
	while (window.isOpen())
	{
		glfwPollEvents();

		int esc = glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE);
		if (esc == GLFW_PRESS)
		{
			window.close();
		}

		int w = glfwGetKey(window.getWindow(), GLFW_KEY_W);
		if (w == GLFW_PRESS)
		{
			camera.onKeyDown({ protostar::Keys::W });
		}
		else
		{
			camera.onKeyUp({ protostar::Keys::W });
		}

		int s = glfwGetKey(window.getWindow(), GLFW_KEY_S);
		if (s == GLFW_PRESS)
		{
			camera.onKeyDown({ protostar::Keys::S });
		}
		else
		{
			camera.onKeyUp({ protostar::Keys::S });
		}

		int a = glfwGetKey(window.getWindow(), GLFW_KEY_A);
		if (a == GLFW_PRESS)
		{
			camera.onKeyDown({ protostar::Keys::A });
		}
		else
		{
			camera.onKeyUp({ protostar::Keys::A });
		}

		int d = glfwGetKey(window.getWindow(), GLFW_KEY_D);
		if (d == GLFW_PRESS)
		{
			camera.onKeyDown({ protostar::Keys::D });
		}
		else
		{
			camera.onKeyUp({ protostar::Keys::D });
		}

		int r = glfwGetKey(window.getWindow(), GLFW_KEY_R);
		if (r == GLFW_PRESS)
		{
			atlasSpr.rotate(0.1f);
			atlasSpr.applyTransforms();
		}
		
		camera.update(1.0);
		shader.setUniform<glm::mat4>("u_cameraProj", camera.getProj());
		shader.setUniform<glm::mat4>("u_cameraView", camera.getTransformation());

		// Render.
		window.begin(qs::Colours::White);

		renderer.drawSprite(atlasSpr, shader);
		renderer.drawText(text, shader);
		//renderer.drawSprite(wall, shader);

		window.end();
	}

	// Exit.
	window.destroy();

	return 0;
}