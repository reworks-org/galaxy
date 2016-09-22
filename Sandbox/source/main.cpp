//
//  main.cpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/utils/Log.hpp"
#include "re/app/Application.hpp"

#include "re/systems/EventSystem.hpp"
#include "re/systems/RenderSystem.hpp"
#include "re/graphics/ui/UISystem.hpp"
#include "re/systems/state/StateSystem.hpp"

#include "re/services/ServiceLocator.hpp"

#include "Menu.hpp"
#include "Game.hpp"

using namespace re;

class App : public re::Application
{
public:
	// See re::Application.
	inline App::App() : Application() {}

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Override and set up application.
	*/
	void Init() override
	{
		RE_LOG_ENABLE(true);
		RE_LOG_ENABLE_FILE(false);

		RE_LOG(re::LogLevel::INFO, "Begin init");

		m_vfs.Mount("bin/Release/assets/data.zip");
		m_config.Parse("bin/Release/config.lua");

		m_appTitle = m_config.Lookup<std::string>("appTitle");
		m_targetUPS = m_config.Lookup<double>("ups");
		m_versionMajor = m_config.Lookup<int>("versionMajor");
		m_versionMinor = m_config.Lookup<int>("versionMinor");
		m_versionPatch = m_config.Lookup<int>("versionPatch");

		m_window.create(sf::VideoMode(m_config.Lookup<int>("screenWidth"), m_config.Lookup<int>("screenHeight")), m_appTitle);

		std::string msg = m_appTitle + " - v" + std::to_string(m_versionMajor) + "." + std::to_string(m_versionMinor) + "." + std::to_string(m_versionPatch);
		RE_LOG(re::LogLevel::INFO, msg);

		m_window.setMouseCursorVisible(m_config.Lookup<int>("cursorVisible"));
		m_window.setVerticalSyncEnabled(m_config.Lookup<int>("vsyncEnabled"));
		m_window.setFramerateLimit(m_config.Lookup<int>("framerateLimit"));

		// create systems
		m_world.AddSystem<StateSystem>(std::make_unique<StateSystem>());
		m_world.AddSystem<RenderSystem>(std::make_unique<RenderSystem>(2));
		m_world.AddSystem<UISystem>(std::make_unique<UISystem>());
		m_world.AddSystem<EventSystem>(std::make_unique<EventSystem>());

		// create states
		m_world.GetSystem<StateSystem>()->RegisterState<Menu>(StateID::menu);
		m_world.GetSystem<StateSystem>()->RegisterState<Game>(StateID::game);

		// provide services
		Locator::Provide<World>(&m_world);
		Locator::Provide<VFS>(&m_vfs);
		Locator::Provide<ConfigReader>(&m_config);
		Locator::Provide<Window>(&m_window);

		// set icon
		m_window.LoadIcon("icon.png");
		
		m_world.GetSystem<StateSystem>()->PushState(StateID::menu);
	}
};

int main()
{
	App app;
	app.Init();
	
	return app.Run();
}