//
//  Application.cpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <iostream>

#include <SFML/Graphics/Shader.hpp>

#include "re/utils/Log.hpp"
#include "re/utils/Time.hpp"
#include "re/systems/StateSystem.hpp"

#include "Application.hpp"

namespace re
{
	Application::Application()
	{
		if (!sf::Shader::isAvailable())
		{
			RE_LOG(LogLevel::FATAL, "Shaders not avaliable on this system");
		}
	}

	int Application::Run()
	{
		//sf::Uint64 lastTime = NanoTime();
		//double delta = 0.0;
		//const double ns = 1000000000.0 / m_targetUPS;
		
		sf::Uint64 timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		int frames = 0;
		int updates = 0;

		sf::Clock clock;
		sf::Time timeSinceLastUpdate = sf::Time::Zero;
		const sf::Time TimePerFrame = sf::seconds(1.f / m_targetUPS);

		m_stateManager.LoadResources();

		while (m_window.isOpen())
		{
			//sf::Uint64 now = NanoTime();

			sf::Time dt = clock.restart();
			timeSinceLastUpdate += dt;

			//delta += (now - lastTime) / ns;
			//lastTime = now;
			
			//if (delta >= 1.0)
			while (timeSinceLastUpdate > TimePerFrame)
			{
				timeSinceLastUpdate -= TimePerFrame;

				m_stateManager.Event(m_window.m_event);
				m_stateManager.Update(TimePerFrame);

				updates++;
				//delta--;
			}
			
			m_stateManager.Render();
			frames++;

			if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timer) > 1000)
			{
				timer += 1000;

			//#if defined _DEBUG || defined DEBUG
				std::cout << updates << " ups, " << frames << " fps" << std::endl;
				std::string header = "  |  " + std::to_string(updates) + " ups, " + std::to_string(frames) + " fps";
				std::string newTitle = m_appTitle + header;
				m_window.setTitle(newTitle);
			//#endif

				updates = 0;
				frames = 0;
			}
		}

		m_stateManager.UnloadResources();
		RE_LOG(LogLevel::INFO, "Engine quit successfully");

		return EXIT_SUCCESS;
	}
}