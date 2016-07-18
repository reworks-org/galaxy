//
//  main.cpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/utils/Log.hpp"
#include "re/app/Application.hpp"

class App : public re::Application
{
public:
	/// See re::Application.
	inline App::App() : Application() {}

	/// See re::Application.
	inline App::App(bool initInConstructor) : Application(initInConstructor) {}

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Override and set up application.
	*/
	inline void Init()
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

		m_window.create(sf::VideoMode(m_config.Lookup<int>("screenWidth"), m_config.Lookup<int>("screenHeight")), m_appTitle);

		std::string msg = m_appTitle + " - v" + std::to_string(m_versionMajor) + "." + std::to_string(m_versionMinor) + "." + std::to_string(m_versionPatch);
		RE_LOG(re::LogLevel::INFO, msg);

		m_window.setIcon();
		m_window.setMouseCursorVisible(true);
		m_window.setVerticalSyncEnabled(false);
		m_window.setFramerateLimit(0);

		// create systems

		// provide services

		// change to first state
		// load resources
	}
};

int main()
{
	App app(true);
	
	return app.Run();
}