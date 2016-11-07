//
//  Application.hpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_APPLICATION_HPP_
#define RENGINE3_APPLICATION_HPP_

#include "re/app/World.hpp"
#include "re/services/Config.hpp"
#include "re/graphics/Window.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/systems/StateManager.hpp"
#include "re/graphics/FontManager.hpp"

namespace re
{
	class Application
	{
	public:
		/*
		* IMPORTS: true or false to enable logging and or file logging.
		* EXPORTS: none
		* PURPOSE: Default constructor. You must call Init() manually.
		*/
		Application(bool enableLogging, bool enableFileLogging);

		/*
		* IMPORTS: configFile - The name of the lua file containing the configuration data for the engine.
		* EXPORTS: none
		* PURPOSE: This is the function you override to set up your application. It is abstract so you must implement it. Set up the VFS first!
		*/
		virtual void Init() = 0;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Runs the application. Return this from the main entry point.
		*/
		int Run();

	protected:
		std::string m_appTitle = "REngine3";
		float m_targetUPS = 60.f;

		int m_versionMajor = 1;
		int m_versionMinor = 0;
		int m_versionPatch = 0;

		VFS m_vfs;
		ConfigReader m_config;
		World m_world;
		Window m_window;
		StateManager m_stateManager;
		FontManager m_fontManager;
	};
}

#endif