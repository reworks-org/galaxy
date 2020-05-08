///
/// StateGame.hpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOX_STATEGAME_HPP_
#define SANDBOX_STATEGAME_HPP_

#include <iostream>

#include <protostar/system/Keys.hpp>
#include <qs/utils/Error.hpp>
#include <qs/core/WindowSettings.hpp>
#include <qs/core/Texture.hpp>
#include <qs/core/Window.hpp>
#include <qs/renderer/Renderer.hpp>
#include <qs/graphics/Camera.hpp>
#include <qs/graphics/Sprite.hpp>
#include <qs/graphics/SpriteBatch.hpp>
#include <qs/core/RenderTexture.hpp>
#include <qs/graphics/TextureAtlas.hpp>
#include <qs/text/Text.hpp>
#include <qs/graphics/Line.hpp>
#include <qs/graphics/Circle.hpp>
#include <qs/renderer/LightSource.hpp>
#include <celestial/UI.hpp>
#include <protostar/async/ThreadPool.hpp>
#include <starmap/Map.hpp>
#include <starlight/Dispatcher.hpp>
#include <pulsar/Log.hpp>
#include <frb/Context.hpp>
#include <frb/audio/Audible.hpp>
#include <celestial/widgets/Image.hpp>
#include <celestial/widgets/Label.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <qs/shaders/Light.hpp>
#include <qs/shaders/Lines.hpp>
#include <qs/shaders/Points.hpp>
#include <qs/shaders/Sprites.hpp>
#include <qs/shaders/SpriteBatches.hpp>
#include <qs/shaders/RenderTextToTexture.hpp>
#include <qs/shaders/RenderToTexture.hpp>
#include <qs/shaders/Widgets.hpp>
#include <protostar/state/State.hpp>

class StateGame : public protostar::State
{
public:
	StateGame() noexcept;
	~StateGame() noexcept override;
	void onPush() noexcept override;
	void onPop() noexcept override;
	void events() noexcept override;
	void update(protostar::ProtectedDouble * deltaTime) noexcept override;
	void render() noexcept override;

private:
	qs::Shader shader;
	qs::Shader rttshader;
	qs::Shader textRTTshader;
	qs::Shader pointShader;
	qs::Shader lineShader;
	qs::Shader spiteBatchShader;
	qs::Shader widgetShader;
	qs::TextureAtlas atlas;
	qs::SpriteBatch atlasSpr;
	qs::RenderTexture* att;
	std::array<qs::Vertex, 4> quadA;
	std::array<qs::Vertex, 4> quadB;
	qs::VertexQuadStorage vqs;
	qs::Text text;
	qs::Font font;
	qs::Camera camera; //left, right, bottom, top
	//qs::Point point;
	//qs::Line line;
	//qs::Circle circle;
	qs::LightSource lightSource;
	protostar::ProtectedDouble updte;
	std::unique_ptr<celestial::UITheme> uitheme;
	std::unique_ptr<celestial::UI> ui;
	qs::Transform* t1;
	qs::Transform* t2;
};

#endif