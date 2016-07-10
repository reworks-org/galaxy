//
//  Application.hpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_APPLICATION_HPP_
#define RENGINE3_APPLICATION_HPP_

#include <SFML/Graphics/RenderWindow.hpp>

#include "re/services/World.hpp"

namespace re
{
	class Application
	{
	public:
		/*
		* IMPORTS: configFile - The name of the lua file containing the configuration data for the engine.
		* EXPORTS: none
		* PURPOSE: This is the function you override to set up your application. It is abstract so you must implement it. Set up the VFS first!
		*/
		virtual void Init() = 0;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Runs the application. Return this from the main entry point.
		*/
		int Run();

	private:
		std::string appTitle = "REngine3";
		double m_targetUPS = 60.0;

		sf::RenderWindow m_window;
	};
}

#endif