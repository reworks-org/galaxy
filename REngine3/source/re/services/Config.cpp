//
//  Config.cpp
//  REngine3
//
//  Created by reworks on 17/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <streambuf>

#include "re/utils/Log.hpp"

#include "Config.hpp"

namespace re
{
	bool ConfigReader::Parse(const std::string& configFile)
	{
		bool success = true;

		// Check if config file exists.
		std::ifstream cf;
		cf.open(configFile);

		if (cf.good())
		{
			// Load script with lua through sol2.
			std::string cfstr((std::istreambuf_iterator<char>(cf)), std::istreambuf_iterator<char>());

			m_lua.script(cfstr);

			cf.close();
		}
		else
		{
			cf.close();

			RE_LOG(LogLevel::WARNING, "Failed to load config file! Creating one...");

			std::ofstream newFile;
			newFile.open(configFile);

			newFile << "config =" << std::endl;
			newFile << "{" << std::endl;
			newFile << "    appTitle = \"Default\"," << std::endl;
			newFile << "    ups = 60.0," << std::endl;
			newFile << "    versionMajor = 1," << std::endl;
			newFile << "    versionMinor = 0," << std::endl;
			newFile << "    versionPatch = 0," << std::endl;
			newFile << "    screenWidth = 640," << std::endl;
			newFile << "    screenHeight = 480," << std::endl;
			newFile << "    fullscreen = 0," << std::endl;
			newFile << "    cursorVisible = 1," << std::endl;
			newFile << "    vsyncEnabled = 0," << std::endl;
			newFile << "    framerateLimit = 0" << std::endl;
			newFile << "}" << std::endl;

			newFile.close();

			std::ifstream cf2;
			cf2.open(configFile);

			// Retry to load script.
			if (cf2.good())
			{
				// Load script with lua through sol2.
				std::string cfstr((std::istreambuf_iterator<char>(cf2)), std::istreambuf_iterator<char>());

				m_lua.script(cfstr);
			}
			else
			{
				RE_LOG(LogLevel::FATAL, "Failed to create config file.");
				success = false;
			}

			cf2.close();
		}

		return success;
	}
}