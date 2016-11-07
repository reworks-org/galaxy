//
//  Application.cpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <iostream>

#include <SFML/Graphics/Shader.hpp>

#include "re/utility/Log.hpp"
#include "re/utility/Time.hpp"
#include "re/systems/StateManager.hpp"

#include "Application.hpp"

namespace re
{
	Application::Application(bool enableLogging, bool enableFileLogging)
	{
		RE_LOG_ENABLE(true);
		RE_LOG_ENABLE_FILE(false);

		RE_LOG(LogLevel::WARNING, "*************************************");
		RE_LOG(LogLevel::WARNING, "    RENGINE3 INITIALIZATION BEGIN    ");
		RE_LOG(LogLevel::WARNING, "*************************************");

		RE_ASSERT(sf::Shader::isAvailable(), "Shaders not avaliable on this system!");

		m_world.Init();
	}

	int Application::Run()
	{
		sf::Uint64 timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		int frames = 0;
		int updates = 0;

		sf::Clock clock;
		sf::Time timeSinceLastUpdate = sf::Time::Zero;
		const sf::Time TimePerFrame = sf::seconds(1.f / m_targetUPS);

		m_stateManager.LoadResources();

		while (m_window.isOpen())
		{
			sf::Time dt = clock.restart();
			timeSinceLastUpdate += dt;
			
			while (timeSinceLastUpdate > TimePerFrame)
			{
				timeSinceLastUpdate -= TimePerFrame;

				// Event
				m_stateManager.Event();

				// Update
				m_stateManager.Update(TimePerFrame);

				updates++;
			}
			
			// Render
			m_stateManager.Render();
			frames++;
			
			if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timer) > 1000)
			{
				timer += 1000;

				std::cout << updates << " ups, " << frames << " fps" << std::endl;
				std::string header = "  |  " + std::to_string(updates) + " ups, " + std::to_string(frames) + " fps";
				std::string newTitle = m_appTitle + header;
				m_window.setTitle(newTitle);

				updates = 0;
				frames = 0;
			}
		}

		m_stateManager.UnloadResources();
		RE_LOG(LogLevel::INFO, "REngine3 quit successfully.");

		return EXIT_SUCCESS;
	}
}