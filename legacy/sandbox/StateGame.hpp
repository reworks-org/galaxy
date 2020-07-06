///
/// StateGame.hpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOX_STATEGAME_HPP_
#define SANDBOX_STATEGAME_HPP_

#include <iostream>

#include <qs/vertex/type/SpriteVertex.hpp>
#include <qs/vertex/type/PrimitiveVertex.hpp>
#include <qs/core/Shader.hpp>
#include <protostar/system/Keys.hpp>
#include <qs/core/WindowSettings.hpp>
#include <qs/texture/Texture.hpp>
#include <qs/core/Window.hpp>
#include <qs/core/Renderer.hpp>
#include <qs/graphics/Camera.hpp>
#include <qs/sprite/Sprite.hpp>
#include <qs/sprite/SpriteBatch.hpp>
#include <qs/texture/RenderTexture.hpp>
#include <qs/graphics/TextureAtlas.hpp>
#include <qs/text/Text.hpp>
#include <qs/graphics/Line.hpp>
#include <qs/graphics/Circle.hpp>
#include <qs/graphics/LightSource.hpp>
#include <protostar/async/ThreadPool.hpp>
#include <starmap/Map.hpp>
#include <starlight/Dispatcher.hpp>
#include <pulsar/Log.hpp>
#include <frb/Context.hpp>
#include <frb/audio/Audible.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <protostar/state/State.hpp>
#include <qs/shaders/Texts.hpp>
#include <starlight/Dispatcher.hpp>
#include <protostar/events/MouseWheelEvent.hpp>

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
	qs::Shader glyphShader;
	qs::Shader pointShader;
	qs::Shader lineShader;
	qs::Shader spiteBatchShader;
	qs::Shader textShader;
	qs::Sprite spriteTest;
	qs::TextureAtlas atlas;
	qs::SpriteBatch atlasSpr;
	qs::BatchedSprite bspr1;
	qs::BatchedSprite bspr2;
	qs::Text text;
	qs::Font font;
	qs::Camera camera; //left, right, bottom, top
	protostar::ProtectedDouble updte;
	
	starlight::Dispatcher dispatch;

	//qs::Point point;
	//qs::Line line;
	//qs::Circle circle;
	//qs::LightSource lightSource;
};

#endif