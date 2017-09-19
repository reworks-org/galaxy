//
//  Application.cpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <chrono>

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

#include "re/utility/Log.hpp"
#include "re/services/StateManager.hpp"

#include "Application.hpp"

void engineConfig(std::ofstream& newFile)
{
	newFile << "appTitle = \"Default\"" << std::endl;
	newFile << "assetPath = \"bin/assets/\"" << std::endl;
	newFile << "ups = 60.0" << std::endl;
	newFile << "versionMajor = 1" << std::endl;
	newFile << "versionMinor = 0" << std::endl;
	newFile << "versionPatch = 0" << std::endl;
	newFile << "screenWidth = 640" << std::endl;
	newFile << "screenHeight = 480" << std::endl;
	newFile << "renderingLayers = 2" << std::endl;
	newFile << "framerateLimit = 0" << std::endl;
	newFile << "keyRepeat = true" << std::endl;
	newFile << "cursorVisible = true" << std::endl;
	newFile << "vsyncEnabled = false" << std::endl;
	newFile << "saveLog = false" << std::endl;
	newFile << "enableDebug = false" << std::endl;
}

namespace re
{
	Application::Application(float32 gravity)
		:m_physicsManager(gravity), m_appTitle(""), m_targetUPS(60.0f), m_saveLog(false), m_enableDebug(false), m_versionMajor(0), m_versionMinor(0), m_versionPatch(1),
		m_engineConfig("config.cfg", engineConfig)
	{
		RE_LOG_PROVIDEWINDOW(&window);
		RE_LOG_PRINTPRETTY(LogLevel::WARNING, "REngine3 Initialization Begin");

		al_init();
		al_init_image_addon();
		al_init_font_addon();
		al_init_ttf_addon();
		al_install_keyboard();
		al_install_mouse();
		al_init_native_dialog_addon();

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

		m_stateManager.load();
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

		m_stateManager.unload();

		RE_LOG(LogLevel::INFO, "Program quit successfully", "Application::run", "Application.cpp", 78);
		RE_LOG_SAVE(m_saveLog);

		return EXIT_SUCCESS;
	}
}
