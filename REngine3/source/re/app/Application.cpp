//
//  Application.cpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <iostream>

#include "re/utils/Log.hpp"
#include "re/utils/Time.hpp"

#include "Application.hpp"

namespace re
{
	int Application::Run()
	{
		uint64 lastTime = TimeNano();
		uint64 timer = TimeMillis();

		const double ns = 1000000000.0 / m_targetUPS;
		double delta = 0.0;
		int frames = 0;
		int updates = 0;

		//load resources

		while (m_window.isOpen())
		{
			uint64 now = TimeNano();
			delta += (now - lastTime) / ns;
			lastTime = now;
			//event
			
			while (delta >= 1)
			{
				//update
				++updates;
				--delta;
			}
			//render
			++frames;

			if ((TimeMillis() - timer) > 1000)
			{
				timer += 1000;

			#if defined _DEBUG || defined DEBUG
				std::cout << updates << " ups, " << frames << " fps" << std::endl;
				std::string header = "  |  " + std::to_string(updates) + " ups, " + std::to_string(frames) + " fps";
				std::string newTitle = appTitle + header;
				m_window.setTitle(newTitle);
			#endif

				updates = 0;
				frames = 0;
			}
		}

		//unload resources

		RE_LOG(LogLevel::INFO, "Engine quit successfully");

		return EXIT_SUCCESS;
	}
}