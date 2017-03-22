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
	App(bool enableLogging, bool enableFileLogging, double gravity) : Application(enableLogging, enableFileLogging, gravity)
    {
        m_vfs.Mount("bin/Release/assets/");
        m_config.Parse("bin/Release/config.lua");
        
        m_appTitle = m_config.Lookup<std::string>("appTitle");
        m_targetUPS = m_config.Lookup<float>("ups");
        m_versionMajor = m_config.Lookup<int>("versionMajor");
        m_versionMinor = m_config.Lookup<int>("versionMinor");
        m_versionPatch = m_config.Lookup<int>("versionPatch");
        
        if (m_config.Lookup<bool>("fullscreen"))
        {
            m_window.create(sf::VideoMode(m_config.Lookup<int>("screenWidth"), m_config.Lookup<int>("screenHeight")), m_appTitle, sf::Style::Default | sf::Style::Fullscreen);
        }
        else
        {
            m_window.create(sf::VideoMode(m_config.Lookup<int>("screenWidth"), m_config.Lookup<int>("screenHeight")), m_appTitle, sf::Style::Default);
        }
        
        std::string msg = m_appTitle + " - v" + std::to_string(m_versionMajor) + "." + std::to_string(m_versionMinor) + "." + std::to_string(m_versionPatch);
        RE_LOG(re::LogLevel::INFO, msg);
        
        m_window.setMouseCursorVisible(m_config.Lookup<bool>("cursorVisible"));
        m_window.setVerticalSyncEnabled(m_config.Lookup<bool>("vsyncEnabled"));
        m_window.setFramerateLimit(0);
        m_window.setKeyRepeatEnabled(true);
        m_window.requestFocus();
        
        // create systems
        m_world.AddSystem<RenderSystem>(std::make_shared<RenderSystem>(m_config.Lookup<int>("renderingLayers")));
        m_world.AddSystem<EventSystem>(std::make_shared<EventSystem>());
        m_world.AddSystem<MoveSystem>(std::make_shared<MoveSystem>());
        m_world.AddSystem<PhysicsSystem>(std::make_shared<PhysicsSystem>(&m_physicsManager, m_targetUPS, 8, 3));
        m_world.AddSystem<AnimationSystem>(std::make_shared<AnimationSystem>());
        m_world.AddSystem<AudioSystem>(std::make_shared<AudioSystem>());
        
        // set up collision listener
        m_physicsManager.m_world.SetContactListener(&m_b2dcallbacks);
        
        // provide services
        Locator::Provide<World>(&m_world);
        Locator::Provide<VFS>(&m_vfs);
        Locator::Provide<ConfigReader>(&m_config);
        Locator::Provide<Window>(&m_window);
        Locator::Provide<FontManager>(&m_fontManager);
        Locator::Provide<StateManager>(&m_stateManager);
        Locator::Provide<Box2DManager>(&m_physicsManager);
        
        // add fonts
        m_fontManager.Add("GameOver", "game_over.ttf");
        
        // set icon
        m_window.LoadIcon("icon.png");
        
        // create states
        m_stateManager.SetState(Load::Inst());
    }

private:
	SandboxContact m_b2dcallbacks;
};

int main()
{
	App app(true, false, 9.81);
	
	return app.run();
}
