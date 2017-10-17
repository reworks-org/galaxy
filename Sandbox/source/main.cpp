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

class Game : public re::Application
{
public:
	Game(const std::string& archive, const std::string& config, std::function<void(std::ofstream&)> newConfig) : re::Application(archive, config, newConfig)
	{
		re::Locator::provide<re::World>(m_world);
		re::Locator::provide<re::VFS>(m_vfs);
		re::Locator::provide<re::StateManager>(m_stateManager);
		re::Locator::provide<re::Box2DManager>(m_b2dManager);
		re::Locator::provide<re::DebugManager>(m_debugManager);
		re::Locator::provide<re::Window>(m_window);
		re::Locator::provide<re::TexturePacker>(m_texturePacker);

		// Register Systems
		m_world->m_systemManager.configure();
	}
};

void newConfigFunc(std::ofstream& newConfig)
{
	newConfig << "appTitle = \"Default\"" << std::endl;
	newConfig << "assetPath = \"bin/assets/\"" << std::endl;
	newConfig << "ups = 60.0" << std::endl;
	newConfig << "versionMajor = 1" << std::endl;
	newConfig << "versionMinor = 0" << std::endl;
	newConfig << "versionPatch = 0" << std::endl;
	newConfig << "screenWidth = 640" << std::endl;
	newConfig << "screenHeight = 480" << std::endl;
	newConfig << "renderingLayers = 2" << std::endl;
	newConfig << "framerateLimit = 0" << std::endl;
	newConfig << "keyRepeat = true" << std::endl;
	newConfig << "cursorVisible = true" << std::endl;
	newConfig << "vsyncEnabled = false" << std::endl;
	newConfig << "saveLog = false" << std::endl;
	newConfig << "enableDebug = false" << std::endl;
}

int main(int argc, char **argv)
{
	Game sandbox("data.archive", "config.lua", newConfigFunc);

	return sandbox.run();
}
