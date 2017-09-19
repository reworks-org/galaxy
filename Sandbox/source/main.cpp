//
//  main.cpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <re/utility/Log.hpp>
#include <re/app/Application.hpp>
#include <re/systems/EventSystem.hpp>
#include <re/systems/RenderSystem.hpp>
#include <re/services/StateManager.hpp>
#include <re/services/ServiceLocator.hpp>
#include <re/systems/PhysicsSystem.hpp>
#include <re/systems/AnimationSystem.hpp>
#include <re/systems/AudioSystem.hpp>

#include "gamesystems/MoveSystem.hpp"
#include "physics/B2DCallbacks.hpp"

#include "Menu.hpp"
#include "Load.hpp"

using namespace re;

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
        Locator::provide<World>(&m_world);
        Locator::provide<VFS>(&m_vfs);
        Locator::provide<Window>(&m_window);
        Locator::provide<ResourceManager<sf::Font>>(&m_fontManager);
		Locator::provide<ResourceManager<sf::Shader>>(&m_shaderManager);
		Locator::provide<ResourceManager<sf::Texture>>(&m_spriteSheetManager);
        Locator::provide<StateManager>(&m_stateManager);
        Locator::provide<PhysicsManager>(&m_physicsManager);
		Locator::provide<DebugManager>(&m_debugManager);
        
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

int main(int argc, char **argv)
{
	App app(9.81f);
	
	return app.run();
}
