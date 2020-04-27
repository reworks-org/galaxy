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
#include <qs/core/RenderTexture.hpp>
#include <qs/graphics/TextureAtlas.hpp>
#include <qs/text/Text.hpp>
#include <qs/graphics/Line.hpp>
#include <qs/graphics/Circle.hpp>
#include <qs/renderer/LightSource.hpp>

#include <starmap/Map.hpp>
#include <starlight/Dispatcher.hpp>
#include <pulsar/Log.hpp>
#include <frb/Context.hpp>
#include <frb/audio/Audible.hpp>

struct Test
{
	int a = 10;
};

[[maybe_unused]] void starmap_func()
{
	try
	{
		starmap::Map map;
		map.load("../demo-maps/demo-zlib.json");
		map.parse();
		map.dump();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

[[maybe_unused]] void starlight_func()
{
	Test test;
	starlight::Dispatcher dispatcher;

	dispatcher.add<Test>([](const Test& test_int, std::mutex& mutex)
		{
			mutex.lock();

			std::cout << test_int.a << std::endl;

			mutex.unlock();
		});

	dispatcher.trigger<Test>(test);
}

[[maybe_unused]] void pulsar_func()
{
	pulsar::Log::get().init("logs/a.txt");
	pulsar::Log::get().setMinimumLevel(pulsar::Log::Level::INFO);

	PL_LOG(PL_INFO, "Should not log unless INFO is min level.");
	PL_LOG(PL_WARNING, "Should Log.");
}

[[maybe_unused]] void frb_func()
{
	frb::Context context;
	context.initialize();
	context.setListenerGain(0.2f);

	frb::Audible music;
	music.load("bin/test.ogg");
	music.play();
	
	std::cin.get();
	music.stop();
}

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
	qs::WindowSettings::s_textureFormat = GL_RGBA8;

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
	shader.loadFromPath("bin/shaders/sprite.glsl");

	qs::Shader rttshader;
	rttshader.loadFromPath("bin/shaders/render_to_texture.glsl");

	qs::Shader textRTTshader;
	textRTTshader.loadFromPath("bin/shaders/render_text_to_texture.glsl");

	qs::Shader pointShader;
	pointShader.loadFromPath("bin/shaders/point.glsl");

	qs::Shader lineShader;
	lineShader.loadFromPath("bin/shaders/line.glsl");

	qs::TextureAtlas atlas;

	atlas.add("bin/wall.png");
	atlas.add("bin/wall_2.png");
	atlas.create(window, renderer, rttshader);
	atlas.save("bin/atlas");

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
	text.create(window, renderer, textRTTshader);
	text.asSprite().save("bin/text");

	shader.bind();

	qs::Camera camera; //left, right, bottom, top
	camera.create(0.0f, window.getWidth(), window.getHeight(), 0.0f);
	camera.setSpeed(0.2f);

	//qs::Point point;
	//point.create(20, 20, 10);

	//qs::Line line;
	//line.create(50, 50, 600, 600, 20);

	//qs::Circle circle;
	//circle.create(100, 100, 200, 200);
	//circle.setThickness(50);

	//atlasSpr.setActiveQuad(atlas.getID("wall"));

	qs::LightSource lightSource;
	lightSource.m_ambientColour = {0.6f, 0.6f, 1.0f, 0.2f};
	lightSource.m_lightColour = {1.0f, 0.8f, 0.6f, 1.0f};
	lightSource.m_falloff = { 0.4f, 3.0f, 20.0f };
	lightSource.m_zLevel = 0.075f;
	lightSource.m_pos = { 500.0f, 200.0f };
	lightSource.m_shader.loadFromPath("bin/shaders/light.vs", "bin/shaders/light.fs");

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

		int q = glfwGetKey(window.getWindow(), GLFW_KEY_Q);
		if (q == GLFW_PRESS)
		{
			atlasSpr.rotate(-0.1f);
			atlasSpr.applyTransforms();
		}

		int e = glfwGetKey(window.getWindow(), GLFW_KEY_E);
		if (e == GLFW_PRESS)
		{
			atlasSpr.rotate(0.1f);
			atlasSpr.applyTransforms();
		}
		
		int g = glfwGetKey(window.getWindow(), GLFW_KEY_G);
		if (g == GLFW_PRESS)
		{
			lightSource.m_falloff.x += 0.001f;
		}

		int b = glfwGetKey(window.getWindow(), GLFW_KEY_B);
		if (b == GLFW_PRESS)
		{
			lightSource.m_falloff.x -= 0.001f;
		}

		int h = glfwGetKey(window.getWindow(), GLFW_KEY_H);
		if (h == GLFW_PRESS)
		{
			lightSource.m_falloff.y += 0.001f;
		}

		int n = glfwGetKey(window.getWindow(), GLFW_KEY_N);
		if (n == GLFW_PRESS)
		{
			lightSource.m_falloff.y -= 0.001f;
		}

		int j = glfwGetKey(window.getWindow(), GLFW_KEY_J);
		if (j == GLFW_PRESS)
		{
			lightSource.m_falloff.z += 0.001f;
		}

		int m = glfwGetKey(window.getWindow(), GLFW_KEY_M);
		if (m == GLFW_PRESS)
		{
			lightSource.m_falloff.z -= 0.001f;
		}

		int i = glfwGetKey(window.getWindow(), GLFW_KEY_I);
		if (i == GLFW_PRESS)
		{
			lightSource.m_pos.x += 0.1f;
		}

		int k = glfwGetKey(window.getWindow(), GLFW_KEY_K);
		if (k == GLFW_PRESS)
		{
			lightSource.m_pos.x -= 0.1f;
		}

		int o = glfwGetKey(window.getWindow(), GLFW_KEY_O);
		if (o == GLFW_PRESS)
		{
			lightSource.m_pos.y += 0.1f;
		}

		int l = glfwGetKey(window.getWindow(), GLFW_KEY_L);
		if (l == GLFW_PRESS)
		{
			lightSource.m_pos.y -= 0.1f;
		}

		camera.update(1.0);
		
		//shader.setUniform<glm::mat4>("u_cameraProj", camera.getProj());
		//shader.setUniform<glm::mat4>("u_cameraView", camera.getTransformation());
		
		// Render.
		window.begin(qs::Colours::Black);

		//pointShader.bind();
		//pointShader.setUniform<glm::mat4>("u_cameraProj", camera.getProj());
		//pointShader.setUniform<glm::mat4>("u_cameraView", camera.getTransformation());
		//pointShader.setUniform("u_colour", 1.0f, 1.0f, 1.0f, 1.0f);
		//renderer.drawPoint(point, pointShader);

		//lineShader.bind();
		//lineShader.setUniform<glm::mat4>("u_cameraProj", camera.getProj());
		//lineShader.setUniform<glm::mat4>("u_cameraView", camera.getTransformation());
		//lineShader.setUniform("u_colour", 1.0f, 1.0f, 1.0f, 1.0f);
		//renderer.drawLine(line);
		
		// Uses same shader as line shader.
		//renderer.drawCircle(circle);

		renderer.drawScene(atlasSpr, camera, lightSource);
		//renderer.drawSprite(atlasSpr, shader);
		//renderer.drawText(text, shader);

		window.end();
	}

	// Exit.
	window.destroy();

	return 0;
}