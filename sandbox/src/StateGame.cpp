///
/// StateGame.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#include "StateGame.hpp"

struct Test
{
	int a = 10;
};

[[maybe_unused]] void starmap_func()
{
	starmap::Map map;
	if (!map.load("../demo-maps/demo-zlib.json"))
	{
		PL_LOG(PL_ERROR, "failed to load map");
	}
	else
	{
		map.parse();
		map.dump();
	}
}

[[maybe_unused]] void starlight_func()
{
	Test test;
	starlight::Dispatcher dispatcher;

	dispatcher.add<Test>([](const Test& test_int)
		{
			std::cout << test_int.a << std::endl;
		});

	dispatcher.trigger<Test>(test);
}

[[maybe_unused]] void pulsar_func()
{
	pulsar::Log::get().init("logs/a.txt");
	pulsar::Log::get().setMinimumLevel(PL_INFO);

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

StateGame::StateGame() noexcept
{
	// Shaders
	shader.loadFromRaw(qs::s_spriteVS, qs::s_spriteFS);
	rttshader.loadFromRaw(qs::s_renderToTextureVS, qs::s_renderToTextureFS);
	textRTTshader.loadFromRaw(qs::s_renderTextToTextureVS, qs::s_renderTextToTextureFS);
	pointShader.loadFromRaw(qs::s_pointsVS, qs::s_pointsFS);
	lineShader.loadFromRaw(qs::s_linesVS, qs::s_linesFS);
	spiteBatchShader.loadFromRaw(qs::s_spriteBatchesVS, qs::s_spriteBatchesFS);
	widgetShader.loadFromRaw(qs::s_widgetVS, qs::s_widgetFS);

	auto* window = SL_HANDLE.window();
	auto* renderer = SL_HANDLE.renderer();
	
	atlas.add("bin/wall.png");
	atlas.add("bin/wall_2.png");
	rttshader.bind();
	atlas.create(*window, *renderer, rttshader);
	//atlas.save("bin/atlas");

	att = &atlas.getTexture();
	atlasSpr.load(att->getGLTexture(), att->getWidth(), att->getHeight());

	auto wq = atlas.getTexQuad("wall");
	auto wq2 = atlas.getTexQuad("wall_2");

	quadA = qs::Vertex::make_quad(
		{ 0.0f, 0.0f, wq.m_width, wq.m_height },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
		wq.m_x, wq.m_y
	);
	quadB = qs::Vertex::make_quad(
		{ 0.0f, 0.0f, wq2.m_width, wq2.m_height },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
		wq2.m_x, wq2.m_y
	);

	vqs.push_back(quadA);
	vqs.push_back(quadB);

	atlasSpr.create(vqs);

	// quad a vertex is from 0 - 3.
	t1 = atlasSpr.getTransform(0);
	t1->move(0.0f, 0.0f);

	// quad b vertex is from 4 - 7.
	t2 = atlasSpr.getTransform(1);
	t2->move(500.0f, 500.0f);

	textRTTshader.bind();
	font.create("bin/public.ttf", 36);
	text.load("HELLO, WORLD.", &font, { 0, 0, 0, 255 });
	text.create(*window, *renderer, textRTTshader);
	//text.asSprite().save("bin/text");

	camera.create(0.0f, window->getWidth(), window->getHeight(), 0.0f);

	//point.create(20, 20, 10);
	//line.create(50, 50, 600, 600, 20);
	//circle.create(100, 100, 200, 200);
	//circle.setThickness(50);

	lightSource.m_ambientColour = { 0.6f, 0.6f, 1.0f, 0.2f };
	lightSource.m_lightColour = { 1.0f, 0.8f, 0.6f, 1.0f };
	lightSource.m_falloff = { 0.4f, 3.0f, 20.0f };
	lightSource.m_zLevel = 0.075f;
	lightSource.m_pos = { 500.0f, 200.0f };
	lightSource.m_shader.loadFromRaw(qs::s_lightVS, qs::s_lightFS);
	
	updte.set(1.0);
}

StateGame::~StateGame() noexcept
{
}

void StateGame::onPush() noexcept
{
}

void StateGame::onPop() noexcept
{
}

void StateGame::events() noexcept
{
	auto* window = SL_HANDLE.window();

	int esc = glfwGetKey(window->getWindow(), GLFW_KEY_ESCAPE);
	if (esc == GLFW_PRESS)
	{
		window->close();
	}

	int w = glfwGetKey(window->getWindow(), GLFW_KEY_W);
	if (w == GLFW_PRESS)
	{
		camera.onKeyDown(protostar::KeyDownEvent{ protostar::Keys::W });
	}
	else
	{
		camera.onKeyUp(protostar::KeyUpEvent{ protostar::Keys::W });
	}

	int s = glfwGetKey(window->getWindow(), GLFW_KEY_S);
	if (s == GLFW_PRESS)
	{
		camera.onKeyDown(protostar::KeyDownEvent{ protostar::Keys::S });
	}
	else
	{
		camera.onKeyUp(protostar::KeyUpEvent{ protostar::Keys::S });
	}

	int a = glfwGetKey(window->getWindow(), GLFW_KEY_A);
	if (a == GLFW_PRESS)
	{
		camera.onKeyDown(protostar::KeyDownEvent{ protostar::Keys::A });
	}
	else
	{
		camera.onKeyUp(protostar::KeyUpEvent{ protostar::Keys::A });
	}

	int d = glfwGetKey(window->getWindow(), GLFW_KEY_D);
	if (d == GLFW_PRESS)
	{
		camera.onKeyDown(protostar::KeyDownEvent{ protostar::Keys::D });
	}
	else
	{
		camera.onKeyUp(protostar::KeyUpEvent{ protostar::Keys::D });
	}

	int q = glfwGetKey(window->getWindow(), GLFW_KEY_Q);
	if (q == GLFW_PRESS)
	{
		t1->rotate(-0.1f);
	}

	int e = glfwGetKey(window->getWindow(), GLFW_KEY_E);
	if (e == GLFW_PRESS)
	{
		t1->rotate(0.1f);
	}

	int g = glfwGetKey(window->getWindow(), GLFW_KEY_G);
	if (g == GLFW_PRESS)
	{
		lightSource.m_falloff.x += 0.001f;
	}

	int b = glfwGetKey(window->getWindow(), GLFW_KEY_B);
	if (b == GLFW_PRESS)
	{
		lightSource.m_falloff.x -= 0.001f;
	}

	int h = glfwGetKey(window->getWindow(), GLFW_KEY_H);
	if (h == GLFW_PRESS)
	{
		lightSource.m_falloff.y += 0.001f;
	}

	int n = glfwGetKey(window->getWindow(), GLFW_KEY_N);
	if (n == GLFW_PRESS)
	{
		lightSource.m_falloff.y -= 0.001f;
	}

	int j = glfwGetKey(window->getWindow(), GLFW_KEY_J);
	if (j == GLFW_PRESS)
	{
		lightSource.m_falloff.z += 0.001f;
	}

	int m = glfwGetKey(window->getWindow(), GLFW_KEY_M);
	if (m == GLFW_PRESS)
	{
		lightSource.m_falloff.z -= 0.001f;
	}

	int i = glfwGetKey(window->getWindow(), GLFW_KEY_I);
	if (i == GLFW_PRESS)
	{
		lightSource.m_pos.x += 0.1f;
	}

	int k = glfwGetKey(window->getWindow(), GLFW_KEY_K);
	if (k == GLFW_PRESS)
	{
		lightSource.m_pos.x -= 0.1f;
	}

	int o = glfwGetKey(window->getWindow(), GLFW_KEY_O);
	if (o == GLFW_PRESS)
	{
		lightSource.m_pos.y += 0.1f;
	}

	int l = glfwGetKey(window->getWindow(), GLFW_KEY_L);
	if (l == GLFW_PRESS)
	{
		lightSource.m_pos.y -= 0.1f;
	}
}

void StateGame::update(protostar::ProtectedDouble* deltaTime) noexcept
{
	camera.update(updte.get());
}

void StateGame::render() noexcept
{
	auto* window = SL_HANDLE.window();
	auto* renderer = SL_HANDLE.renderer();

	// Render.
	window->begin(protostar::White);

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

	renderer->drawScene(atlasSpr, camera, lightSource);

	//spiteBatchShader.bind();
	//spiteBatchShader.setUniform<glm::mat4>("u_cameraProj", camera.getProj());
	//spiteBatchShader.setUniform<glm::mat4>("u_cameraView", camera.getTransformation());
	//renderer->drawSpriteBatch(atlasSpr, spiteBatchShader);

	//shader.bind();
	//shader.setUniform<glm::mat4>("u_cameraProj", camera.getProj());
	//shader.setUniform<glm::mat4>("u_cameraView", camera.getTransformation());
	//renderer.drawText(text, shader);

	window->end();
}