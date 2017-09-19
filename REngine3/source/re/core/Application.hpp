//
//  Application.hpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_APPLICATION_HPP_
#define RENGINE3_APPLICATION_HPP_

#include "re/core/World.hpp"
#include "re/services/VFS.hpp"
#include "re/services/Config.hpp"
#include "re/graphics/Window.hpp"
#include "re/managers/DebugManager.hpp"
#include "re/managers/StateManager.hpp"
#include "re/managers/PhysicsManager.hpp"
#include "re/utility/ResourceManager.hpp"

namespace re
{
	class Application
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Sets up the engine. You need to inherit this and call it from a subclass.
		/// 
		/// \param gravity Sets up the default gravity for Box2D.
		/// 
		Application(float32 gravity);

		//
		/// Cleans up engine.
		/// 
		~Application();

		///
		/// Return app->run() from your main method.
		///
		/// \return Returns EXIT_SUCCESS or EXIT_FAILURE.
		/// 
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
