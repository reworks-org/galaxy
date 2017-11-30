//
//  main.cpp
//  Sandbox
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <iostream>

#include <re/core/Application.hpp>
#include <re/services/ServiceLocator.hpp>

#include <re/components/AnimationComponent.hpp>
#include <re/components/PhysicsComponent.hpp>
#include <re/components/RenderableComponent.hpp>
#include <re/components/SpriteComponent.hpp>
#include <re/components/TextComponent.hpp>
#include <re/components/TransformComponent.hpp>
#include <re/systems/AnimationSystem.hpp>

#include <re/systems/RenderSystem.hpp>
#include <re/systems/PhysicsSystem.hpp>

#include <re/mapping/TMXHelper.hpp>

#include "gamesystems/MoveSystem.hpp"

#include "Test.hpp"

class Game : public re::Application
{
public:
	Game(const std::string& archive, const std::string& config, std::function<void(std::ofstream&)> newConfig) : re::Application(archive, config, newConfig)
	{
		m_world->registerComponent<re::AnimationComponent>("AnimationComponent");
		m_world->registerComponent<re::PhysicsComponent>("PhysicsComponent");
		m_world->registerComponent<re::RenderableComponent>("RenderableComponent");
		m_world->registerComponent<re::SpriteComponent>("SpriteComponent");
		m_world->registerComponent<re::TextComponent>("TextComponent");
		m_world->registerComponent<re::TransformComponent>("TransformComponent");
		m_world->registerComponent<re::AnimationComponent>("AnimationComponent");

		m_world->m_systemManager.add<re::AnimationSystem>();
		m_world->m_systemManager.add<re::RenderSystem>(2);
		m_world->m_systemManager.add<re::PhysicsSystem>(m_configReader->lookup<float>(config, "box2d", "ups"), m_configReader->lookup<int32>(config, "box2d", "velocityIterations"), m_configReader->lookup<int32>(config, "box2d", "positionIterations"));
		m_world->m_systemManager.add<MoveSystem>("player");
		m_world->m_systemManager.configure();
		
		m_world->m_systemManager.system<re::RenderSystem>()->registerRenderableComponents<re::TextComponent, re::SpriteComponent>();

		re::tmx::LINE_THICKNESS = 2.5f;
		re::tmx::setUpLoaders();

		m_debugManager->specifyReloadState(Test::inst(), []() { LOG_S(INFO) << "RESETTING STATE!" << std::endl; });
		m_stateManager->setState(Test::inst());
	}
};

int main(int argc, char **argv)
{
	Game sandbox("bin/data.zip", "config.cfg", [](std::ofstream& newConfig)
	{
		newConfig << "[graphics]" << std::endl;
		newConfig << "width = 1280" << std::endl;
		newConfig << "height = 720" << std::endl;
		newConfig << "fullscreen = false" << std::endl;
		newConfig << "msaa = true" << std::endl;
		newConfig << "msaaValue = 2" << std::endl;
		newConfig << "title = Sandbox" << std::endl;
		newConfig << "icon = icon.png" << std::endl;
		newConfig << "atlas = test.atlas" << std::endl;
		newConfig << std::endl;

		newConfig << "[box2d]" << std::endl;
		newConfig << "gravity = 9.81" << std::endl;
		newConfig << "ups = 60.0" << std::endl;
		newConfig << "velocityIterations = 8" << std::endl;
		newConfig << "positionIterations = 3" << std::endl;
		newConfig << std::endl;

		newConfig << "[fontmanager]" << std::endl;
		newConfig << "fontScript = fonts.lua" << std::endl;
		newConfig << std::endl;

		newConfig << "[audiomanager]" << std::endl;
		newConfig << "audioScript = audio.lua" << std::endl;
		newConfig << "reserveSamples = 16" << std::endl;
	});

	return sandbox.run();
}