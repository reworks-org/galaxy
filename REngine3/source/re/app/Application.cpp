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
		m_debugManager.init(m_window, m_enableDebug);

		while (m_window.isOpen())
		{
			sf::Time dt = clock.restart();
			timeSinceLastUpdate += dt;
            
			while (timeSinceLastUpdate > TimePerFrame)
			{
				timeSinceLastUpdate -= TimePerFrame;
				
                // Handle events that need to be polled
                while (m_window.pollEvent(m_window.m_event))
                {
                    switch(m_window.m_event.type)
                    {
                        case sf::Event::Closed:
                            m_window.close();
                            break;
                            
                        case sf::Event::KeyReleased:
                            switch(m_window.m_event.key.code)
                            {

                            }
                            break;
                    }
                    
                    m_debugManager.event(m_window.m_event);
                    m_stateManager.handlePollEvents(m_window.m_event);
                }
                
                m_stateManager.handleEvents(m_window.m_event);
                
				// Update
				m_debugManager.update(m_window, TimePerFrame);
				m_stateManager.update(TimePerFrame);

                // Display the debug manager
                m_debugManager.useMenu();
                
				updates++;
            }
            
			// Render
			m_window.clear(sf::Color::Black);

			m_stateManager.render();
			m_debugManager.render(m_window);

			m_window.display();
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
		m_debugManager.cleanup();

		RE_LOG(LogLevel::INFO, "Program quit successfully", "Application::run", "Application.cpp", 78);
		RE_LOG_SAVE(m_saveLog);

		return EXIT_SUCCESS;
	}
}
