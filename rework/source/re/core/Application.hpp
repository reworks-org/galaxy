//
//  Application.hpp
//  rework
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_APPLICATION_HPP_
#define REWORK_APPLICATION_HPP_

/*
#include "re/core/World.hpp"
#include "re/services/VFS.hpp"
#include "re/services/Config.hpp"
#include "re/graphics/Window.hpp"
#include "re/managers/FontManager.hpp"
#include "re/managers/StateManager.hpp"
#include "re/managers/DebugManager.hpp"
#include "re/managers/Box2DManager.hpp"
*/

#include <string>

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
		Application();

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

	private:
		//int m_versionMajor;
		//int m_versionMinor;
		//int m_versionPatch;
		double m_ups;
		double m_timePerFrame;
		//bool m_saveLog;
		std::string m_appTitle;

		//VFS m_vfs;
		//Window m_window;
		//World m_world;
		//StateManager m_stateManager;
		//FontManager m_fontManager;
		//Box2DManager m_b2dManager;
		//DebugManager m_debugManager;
	};
}

#endif