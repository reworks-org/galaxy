//
//  Application.hpp
//  rework
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_APPLICATION_HPP_
#define REWORK_APPLICATION_HPP_

#include "re/core/World.hpp"
#include "re/services/VFS.hpp"
#include "re/services/ConfigReader.hpp"
#include "re/graphics/Window.hpp"
#include "re/managers/FontManager.hpp"
#include "re/managers/AudioManager.hpp"
#include "re/managers/StateManager.hpp"
#include "re/managers/Box2DManager.hpp"
#include "re/managers/DebugManager.hpp"
#include "re/graphics/TextureAtlas.hpp"

namespace re
{
	class Application
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Sets up the engine. You need to inherit this and call it from a subclass.
		/// YOU MUST CALL m_world->m_systemManager.configure() after adding custom systems.
		/// THIS CONSTRUCTOR DOES NOT CALL IT.
		/// Also calls std::srand(std::time(nullptr)) for you.
		/// 
		/// \param archive Path or archive to mount.
		/// \param config Path to the config file.
		/// \param newConfig A function pointer to a function that contains ofstream code to write a default config file.
		///
		Application(const std::string& archive, const std::string& config, std::function<void(std::ofstream&)> newConfig);

		//
		/// Cleans up engine.
		/// 
		~Application();

		///
		/// Return app->run() from your main method.
		///
		/// \return Returns EXIT_SUCCESS.
		/// 
		int run();

	protected:
		VFS* m_vfs;
		ConfigReader* m_configReader;
		Window* m_window;
		World* m_world;
		StateManager* m_stateManager;
		FontManager* m_fontManager;
		AudioManager* m_audioManager;
		Box2DManager* m_b2dManager;
		DebugManager* m_debugManager;
		TextureAtlas* m_textureAtlas;
	};
}

#endif