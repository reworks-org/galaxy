//
//  Config.cpp
//  REngine3
//
//  Created by reworks on 17/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <fstream>
#include <iostream>

#include "re/utils/Log.hpp"
#include "re/services/ServiceLocator.hpp"

#include "Config.hpp"

namespace re
{
	bool ConfigReader::Parse(const std::string& configFile)
	{
		bool success = true;

		// Check if config file exists.
		if (Locator::Get<VFS>()->DoesExist(configFile))
		{
			// Load script with lua through sol2.
			lua.script(Locator::Get<VFS>()->GetString(configFile));
		}
		else
		{
			RE_LOG(LogLevel::WARNING, "Failed to load config file! Creating one...");

			std::ofstream newFile{ configFile };

			newFile << "config =" << std::endl;
			newFile << "{" << std::endl;
			newFile << "    title = 'Default'" << std::endl;
			newFile << "    versionMajor = 1" << std::endl;
			newFile << "    versionMinor = 0" << std::endl;
			newFile << "    versionPatch = 0" << std::endl;
			newFile << "    windowWidth = 640" << std::endl;
			newFile << "    windowHeight = 480" << std::endl;
			newFile << "    targetUPS = 60.0" << std::endl;
			newFile << "    fullscreen = 0" << std::endl;
			newFile << "    borderless = 0" << std::endl;
			newFile << "    logging = 1" << std::endl;
			newFile << "}" << std::endl;

			newFile.close();

			// Retry to load script.
			if (Locator::Get<VFS>()->DoesExist(configFile))
			{
				lua.script(Locator::Get<VFS>()->GetString(configFile));
			}
			else
			{
				RE_LOG(LogLevel::FATAL, "Failed to create config file.");
				success = false;
			}
		}

		return success;
	}
}