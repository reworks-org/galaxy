//
//  Application.cpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <chrono>
#include <SFML/Graphics/Shader.hpp>

#include "re/utility/Log.hpp"
#include "re/systems/StateManager.hpp"

#include "Application.hpp"

namespace re
{
	Application::Application(bool enableLogging, bool enableFileLogging, float32 gravity)
		:m_physicsManager(gravity)
	{
		RE_LOG_PRINTPRETTY(LogLevel::WARNING, "REngine3 Initialization Begin");

        RE_ASSERT(sf::Shader::isAvailable(), "Shaders not avaliable on this system", "Application::Application", "Application.cpp", 23);

		m_world.init();
	}

	int Application::run()
	{
		sf::Uint64 timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		int frames = 0;
		int updates = 0;

		sf::Clock clock;
		sf::Time timeSinceLastUpdate = sf::Time::Zero;
		const sf::Time TimePerFrame = sf::seconds(1.f / m_targetUPS);

		m_stateManager.loadResources();

		while (m_window.isOpen())
		{
			sf::Time dt = clock.restart();
			timeSinceLastUpdate += dt;
			
			while (timeSinceLastUpdate > TimePerFrame)
			{
				timeSinceLastUpdate -= TimePerFrame;

				// Event
				m_stateManager.event();

				// Update
				m_stateManager.update(TimePerFrame);

				updates++;
			}
			
			// Render
			m_stateManager.render();
			frames++;
			
			if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timer) > 1000)
			{
				timer += 1000;

				std::string header = m_appTitle + "  |  " + std::to_string(updates) + " ups, " + std::to_string(frames) + " fps";

                RE_LOG(LogLevel::INFO, header, "Application::run", "Application.cpp", 69);
				m_window.setTitle(header);

				updates = 0;
				frames = 0;
			}
		}

		m_stateManager.unloadResources();

		RE_LOG(LogLevel::INFO, "Program quit successfully", "Application::run", "Application.cpp", 78);
		RE_LOG_SAVE(m_saveLog);

		return EXIT_SUCCESS;
	}
}