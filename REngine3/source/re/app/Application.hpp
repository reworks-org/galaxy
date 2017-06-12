//
//  Application.hpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_APPLICATION_HPP_
#define RENGINE3_APPLICATION_HPP_

#include <SFML/Graphics/Font.hpp>

#include "re/app/World.hpp"
#include "re/services/Config.hpp"
#include "re/graphics/Window.hpp"
#include "re/services/VFS.hpp"
#include "re/debug/DebugManager.hpp"
#include "re/systems/StateManager.hpp"
#include "re/physics/Box2DManager.hpp"
#include "re/utility/ResourceManager.hpp"

namespace re
{
	class Application
	{
	public:
		/*
		* IMPORTS: true or false to enable logging and or file logging.
		* EXPORTS: none
		* PURPOSE: Default constructor. Your child class will need to set up the rest of the engine.
		*/
		Application(bool enableLogging, bool enableFileLogging, float32 gravity);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Runs the application. Return this from the main entry point.
		*/
		int run();

	protected:
		std::string m_appTitle = "";
		float m_targetUPS = 60.f;
		bool m_saveLog = false;
		bool m_enableDebug = false;

		int m_versionMajor = 1;
		int m_versionMinor = 0;
		int m_versionPatch = 0;

		VFS m_vfs;
		ConfigReader m_config;
		World m_world;
		Window m_window;
		StateManager m_stateManager;
		ResourceManager<sf::Font> m_fontManager;
		ResourceManager<sf::Shader> m_shaderManager;
		ResourceManager<sf::Texture> m_spriteSheetManager;
		Box2DManager m_physicsManager;
		DebugManager m_debugManager;
	};
}

#endif
