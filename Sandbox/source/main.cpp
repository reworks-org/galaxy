//
//  main.cpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/utils/Log.hpp"
#include "re/app/Application.hpp"
#include "re/systems/state/StateSystem.hpp"
#include "Menu.hpp"

using namespace re;

class App : public re::Application
{
public:
	/// See re::Application.
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

		m_vfs.Mount("bin/Debug/assets/");

		m_config.Parse("config.lua");
		m_appTitle = m_config.Lookup<std::string>("appTitle");
		m_targetUPS = m_config.Lookup<double>("ups");
		m_versionMajor = m_config.Lookup<int>("versionMajor");
		m_versionMinor = m_config.Lookup<int>("versionMinor");
		m_versionPatch = m_config.Lookup<int>("versionPatch");

		m_world.m_window.create(sf::VideoMode(m_config.Lookup<int>("screenWidth"), m_config.Lookup<int>("screenHeight")), m_appTitle);

		std::string msg = m_appTitle + " - v" + std::to_string(m_versionMajor) + "." + std::to_string(m_versionMinor) + "." + std::to_string(m_versionPatch);
		RE_LOG(re::LogLevel::INFO, msg);

		sf::Image tempIcon;
		tempIcon.loadFromStream(m_vfs.ToStream("icon.png"));
		m_world.m_window.setIcon(tempIcon.getSize().x, tempIcon.getSize().y, tempIcon.getPixelsPtr());

		m_world.m_window.setMouseCursorVisible(m_config.Lookup<bool>("cursorVisible"));
		m_world.m_window.setVerticalSyncEnabled(m_config.Lookup<bool>("vsyncEnabled"));
		m_world.m_window.setFramerateLimit(m_config.Lookup<int>("framerateLimit"));

		// create systems
		m_world.AddSystem<StateSystem>(std::unique_ptr<StateSystem>(new StateSystem));

		// create states
		m_world.GetSystem<StateSystem>()->RegisterState<Menu>(StateID::menu);

		// provide services

		m_world.GetSystem<StateSystem>()->PushState(StateID::game);
		// load resources
	}
};

int main()
{
	App app;
	app.Init();
	
	return app.Run();
}