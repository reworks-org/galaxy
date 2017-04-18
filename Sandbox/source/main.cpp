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
#include <re/systems/StateManager.hpp>
#include <re/services/ServiceLocator.hpp>
#include <re/systems/PhysicsSystem.hpp>
#include <re/systems/AnimationSystem.hpp>
#include <re/systems/AudioSystem.hpp>

#include "gamesystems/MoveSystem.hpp"
#include "physics/B2DCallbacks.hpp"

#include "Load.hpp"

using namespace re;

class App : public re::Application
{
public:
	// See re::Application.
	App(bool enableLogging, bool enableFileLogging, float32 gravity) : Application(enableLogging, enableFileLogging, gravity)
    {
		#ifdef _WIN32
			m_vfs.mount("bin/assets/");
			m_config.parse("bin/config.lua");
		#else
			m_vfs.mount("Sandbox.app/Contents/Resources/");
			m_config.parse("Sandbox.app/Contents/config.lua");
		#endif
        
        m_appTitle = m_config.lookup<std::string>("appTitle");
        m_targetUPS = m_config.lookup<float>("ups");
        m_versionMajor = m_config.lookup<int>("versionMajor");
        m_versionMinor = m_config.lookup<int>("versionMinor");
        m_versionPatch = m_config.lookup<int>("versionPatch");
        
        if (m_config.lookup<bool>("fullscreen"))
        {
            m_window.create(sf::VideoMode(m_config.lookup<int>("screenWidth"), m_config.lookup<int>("screenHeight")), m_appTitle, sf::Style::Default | sf::Style::Fullscreen);
        }
        else
        {
            m_window.create(sf::VideoMode(m_config.lookup<int>("screenWidth"), m_config.lookup<int>("screenHeight")), m_appTitle, sf::Style::Default);
        }
        
        std::string msg = m_appTitle + " - v" + std::to_string(m_versionMajor) + "." + std::to_string(m_versionMinor) + "." + std::to_string(m_versionPatch);
        RE_LOG(re::LogLevel::INFO, msg, "App::App", "main.cpp", 51);
        
        m_window.setMouseCursorVisible(m_config.lookup<bool>("cursorVisible"));
        m_window.setVerticalSyncEnabled(m_config.lookup<bool>("vsyncEnabled"));
        m_window.setFramerateLimit(m_config.lookup<int>("framerateLimit"));
        m_window.setKeyRepeatEnabled(m_config.lookup<bool>("keyRepeat"));
        m_window.requestFocus();
        
        // create systems
        m_world.addSystem<RenderSystem>(std::make_shared<RenderSystem>(m_config.lookup<int>("renderingLayers")));
        m_world.addSystem<EventSystem>(std::make_shared<EventSystem>());
        m_world.addSystem<MoveSystem>(std::make_shared<MoveSystem>());
        m_world.addSystem<PhysicsSystem>(std::make_shared<PhysicsSystem>(&m_physicsManager, m_targetUPS, 8, 3));
        m_world.addSystem<AnimationSystem>(std::make_shared<AnimationSystem>());
        m_world.addSystem<AudioSystem>(std::make_shared<AudioSystem>());
        
        // set up collision listener
        m_physicsManager.m_world->SetContactListener(&m_b2dcallbacks);
        
        // provide services
        Locator::provide<World>(&m_world);
        Locator::provide<VFS>(&m_vfs);
        Locator::provide<ConfigReader>(&m_config);
        Locator::provide<Window>(&m_window);
        Locator::provide<FontManager>(&m_fontManager);
        Locator::provide<StateManager>(&m_stateManager);
        Locator::provide<Box2DManager>(&m_physicsManager);
        
        // add fonts
        m_fontManager.add("GameOver", "game_over.ttf");
        
        // set icon
        m_window.loadIcon("icon.png");
        
        // create states
        m_stateManager.setState(Load::inst());
    }

private:
	SandboxContact m_b2dcallbacks;
};

int main()
{
	App app(true, false, (float32)9.81);
	
	return app.run();
}
