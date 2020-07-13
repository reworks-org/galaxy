///
/// StateGame.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#include "StateGame.hpp"

void scrollCallback(GLFWwindow* window, double x, double y) noexcept
{
	//dispatch.trigger<pr::MouseWheelEvent>({ x, y });
}

StateGame::StateGame() noexcept
	:atlasSpr(100)
{
	// Shaders
	//shader.loadFromRaw(qs::s_spriteVS, qs::s_spriteFS);
	//rttshader.loadFromRaw(qs::s_renderToTextureVS, qs::s_renderToTextureFS);
	//glyphShader.loadFromRaw(qs::s_glyphsVS, qs::s_glyphsFS);
	//pointShader.loadFromRaw(qs::s_pointsVS, qs::s_pointsFS);
	//lineShader.loadFromRaw(qs::s_linesVS, qs::s_linesFS);
	//spiteBatchShader.loadFromRaw(qs::s_spriteBatchesVS, qs::s_spriteBatchesFS);
	//textShader.loadFromRaw(qs::s_textVS, qs::s_textFS);

	auto* window = SL_HANDLE.window();
	auto* renderer = SL_HANDLE.renderer();
	
	spriteTest.load("bin/wall.png");
	spriteTest.create<qs::BufferTypeDynamic>();
	spriteTest.move(500.0f, 20.0f);
	spriteTest.rotate(45.0f);

	atlas.add("bin/wall_2.png");
	atlas.add("bin/wall_3.png");
	rttshader.bind();
	atlas.create(*renderer, rttshader);
	//atlas.save("bin/atlas");

	bspr1.create(atlas.getTexQuad("wall_2"), 1);
	bspr2.create(atlas.getTexQuad("wall_3"), 2);

	bspr1.setPos(500, 500);
	bspr2.setPos(20, 20);

	atlasSpr.setTexture(dynamic_cast<qs::BaseTexture*>(atlas.getTexture()));
	atlasSpr.add(&bspr1);
	atlasSpr.add(&bspr2);

	glyphShader.bind();
	font.load("bin/public.ttf", 36);
	font.create(*renderer, glyphShader);
	font.getTexture()->save("bin/demo");

	text.load("HELLO, WORLD.", &font, { 0, 0, 0, 255 });
	text.create();

	camera.create(0.0f, window->getWidth(), window->getHeight(), 0.0f);
	camera.setSpeed(10.0f);

	//point.create(20, 20, 10);
	//line.create(50, 50, 600, 600, 20);
	//circle.create(100, 100, 200, 200);
	//circle.setThickness(50);

	//lightSource.m_ambientColour = { 0.6f, 0.6f, 1.0f, 0.2f };
	//lightSource.m_lightColour = { 1.0f, 0.8f, 0.6f, 1.0f };
	//lightSource.m_falloff = { 0.4f, 3.0f, 20.0f };
	//lightSource.m_zLevel = 0.075f;
	//lightSource.m_pos = { 500.0f, 200.0f };
	//lightSource.m_shader.loadFromRaw(qs::s_lightVS, qs::s_lightFS);

	dispatch.add<pr::KeyDownEvent>(std::bind(&qs::Camera::onKeyDown, &camera, std::placeholders::_1));
	dispatch.add<pr::KeyUpEvent>(std::bind(&qs::Camera::onKeyUp, &camera, std::placeholders::_1));
	dispatch.add<pr::MouseWheelEvent>(std::bind(&qs::Camera::onMouseScroll, &camera, std::placeholders::_1));

	window->setScrollCallback(scrollCallback);

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

	int esc = glfwGetKey(window->getGLWindow(), GLFW_KEY_ESCAPE);
	if (esc == GLFW_PRESS)
	{
		window->close();
	}

	int w = glfwGetKey(window->getGLWindow(), GLFW_KEY_W);
	if (w == GLFW_PRESS)
	{
		camera.onKeyDown(pr::KeyDownEvent{ pr::Keys::W });
	}
	else
	{
		camera.onKeyUp(pr::KeyUpEvent{ pr::Keys::W });
	}

	int s = glfwGetKey(window->getGLWindow(), GLFW_KEY_S);
	if (s == GLFW_PRESS)
	{
		camera.onKeyDown(pr::KeyDownEvent{ pr::Keys::S });
	}
	else
	{
		camera.onKeyUp(pr::KeyUpEvent{ pr::Keys::S });
	}

	int a = glfwGetKey(window->getGLWindow(), GLFW_KEY_A);
	if (a == GLFW_PRESS)
	{
		camera.onKeyDown(pr::KeyDownEvent{ pr::Keys::A });
	}
	else
	{
		camera.onKeyUp(pr::KeyUpEvent{ pr::Keys::A });
	}

	int d = glfwGetKey(window->getGLWindow(), GLFW_KEY_D);
	if (d == GLFW_PRESS)
	{
		camera.onKeyDown(pr::KeyDownEvent{ pr::Keys::D });
	}
	else
	{
		camera.onKeyUp(pr::KeyUpEvent{ pr::Keys::D });
	}
}

void StateGame::update(pr::ProtectedDouble* deltaTime) noexcept
{
	camera.update(updte.get());
	atlasSpr.update();
}

void StateGame::render() noexcept
{
	auto* window = SL_HANDLE.window();
	auto* renderer = SL_HANDLE.renderer();

	// Render.
	window->begin({ 0, 0, 0, 255 });

	shader.bind();
	shader.setUniform<glm::mat4>("u_cameraProj", camera.getProj());
	shader.setUniform<glm::mat4>("u_cameraView", camera.getTransformation());
	renderer->drawSprite(spriteTest, shader);
	
	spiteBatchShader.bind();
	spiteBatchShader.setUniform<glm::mat4>("u_cameraProj", camera.getProj());
	spiteBatchShader.setUniform<glm::mat4>("u_cameraView", camera.getTransformation());
	atlasSpr.bind();
	renderer->drawSpriteBatch(atlasSpr, spiteBatchShader);

	textShader.bind();
	textShader.setUniform<glm::mat4>("u_cameraProj", camera.getProj());
	textShader.setUniform<glm::mat4>("u_cameraView", camera.getTransformation());
	renderer->drawText(text, textShader);

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

	//renderer->drawScene(atlasSpr, camera, lightSource);

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