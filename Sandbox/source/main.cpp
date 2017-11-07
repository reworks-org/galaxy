//
//  main.cpp
//  Sandbox
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

/*
class App : public re::Application
{
public:
	// See re::Application.
	App(float32 gravity) : Application(gravity)
    {
		#ifdef _WIN32
			m_engineConfig.setPath("bin/");
			m_engineConfig.parse("config.lua");
			m_vfs.setBasePath(m_engineConfig.lookup<std::string>("assetPath"));
		#else
			m_engineConfig.parse("Sandbox.app/Contents/configMac.lua");
			m_vfs.setBasePath(m_engineConfig.lookup<std::string>("assetPath"));
		#endif
        
        m_appTitle = m_engineConfig.lookup<std::string>("appTitle");
        m_targetUPS = m_engineConfig.lookup<float>("ups");
        m_versionMajor = m_engineConfig.lookup<int>("versionMajor");
        m_versionMinor = m_engineConfig.lookup<int>("versionMinor");
        m_versionPatch = m_engineConfig.lookup<int>("versionPatch");
		m_saveLog = m_engineConfig.lookup<bool>("saveLog");
		m_enableDebug = m_engineConfig.lookup<bool>("enableDebug");

        m_window.make(m_engineConfig.lookup<int>("screenWidth"), m_engineConfig.lookup<int>("screenHeight"), m_appTitle, sf::Style::Default, false);
              
        std::string msg = m_appTitle + " - v" + std::to_string(m_versionMajor) + "." + std::to_string(m_versionMinor) + "." + std::to_string(m_versionPatch);
        RE_LOG(re::LogLevel::INFO, msg, "App::App", "main.cpp", 51);
        
        m_window.setMouseCursorVisible(m_engineConfig.lookup<bool>("cursorVisible"));
        m_window.setVerticalSyncEnabled(m_engineConfig.lookup<bool>("vsyncEnabled"));
        m_window.setFramerateLimit(m_engineConfig.lookup<int>("framerateLimit"));
        m_window.setKeyRepeatEnabled(m_engineConfig.lookup<bool>("keyRepeat"));
        m_window.requestFocus();
        
        // create systems
        m_world.addSystem<RenderSystem>(std::make_unique<RenderSystem>(m_engineConfig.lookup<int>("renderingLayers"), &m_window));
        m_world.addSystem<EventSystem>(std::make_unique<EventSystem>());
        m_world.addSystem<MoveSystem>(std::make_unique<MoveSystem>());
        m_world.addSystem<PhysicsSystem>(std::make_unique<PhysicsSystem>(&m_physicsManager, m_targetUPS, 8, 3));
        m_world.addSystem<AnimationSystem>(std::make_unique<AnimationSystem>());
        m_world.addSystem<AudioSystem>(std::make_unique<AudioSystem>(50, 50));
        
        // set up collision listener
        m_physicsManager.m_world->SetContactListener(&m_b2dcallbacks);
        
        // provide services
        
        
        // add fonts
        m_fontManager.add("game_over.ttf", "GameOver");
        
        // set icon
        m_window.loadIcon("icon.png");
        
        // create states Load::inst()
		m_debugManager.specifyReloadState(Load::inst(), []() {});
        m_stateManager.setState(Menu::inst());
    }

private:
	SandboxContact m_b2dcallbacks;
};

*/

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
		m_world->m_systemManager.add<re::PhysicsSystem>(m_engineConfig->lookup<float>("box2d", "ups"), m_engineConfig->lookup<int32>("box2d", "velocityIterations"), m_engineConfig->lookup<int32>("box2d", "positionIterations"));
		m_world->m_systemManager.configure();
		
		m_world->m_systemManager.system<re::RenderSystem>()->registerRenderableComponents<re::TextComponent, re::SpriteComponent>();

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