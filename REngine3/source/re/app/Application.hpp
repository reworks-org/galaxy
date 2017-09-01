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
#include "re/services/VFS.hpp"
#include "re/services/Config.hpp"
#include "re/graphics/Window.hpp"
#include "re/debug/DebugManager.hpp"
#include "re/services/StateManager.hpp"
#include "re/physics/PhysicsManager.hpp"
#include "re/utility/ResourceManager.hpp"

namespace re
{
	class Application
	{
	public:
		/*
		* IMPORTS: gravity of world.
		* EXPORTS: none
		* PURPOSE: Default constructor. Your child class will need to set up the rest of the engine.
		*/
		Application(float32 gravity);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Runs the application. Return this from the main entry point.
		*/
		int run();

	protected:
		std::string m_appTitle;
		float m_targetUPS;
		bool m_saveLog;
		bool m_enableDebug;

		int m_versionMajor;
		int m_versionMinor;
		int m_versionPatch;

		VFS m_vfs;
		ConfigReader m_engineConfig;
		World m_world;
		Window m_window;
		StateManager m_stateManager;
		ResourceManager<sf::Font> m_fontManager;
		ResourceManager<sf::Shader> m_shaderManager;
		ResourceManager<sf::Texture> m_spriteSheetManager;
		PhysicsManager m_physicsManager;
		DebugManager m_debugManager;
	};
}

#endif
