//
//  Config.cpp
//  REngine3
//
//  Created by reworks on 17/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>
#include <fstream>
#include <iostream>
#include <streambuf>

#include "re/utility/Log.hpp"

#include "Config.hpp"

namespace re
{
	void ConfigReader::writeTableToFile(const std::string& tableName)
	{
		// Get a table with the components.
		sol::table config = m_lua.get<sol::table>(tableName);

		// Get key-value pairs from table
		std::map<std::string, std::string> m_keyValuePair;
		config.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<std::string>() });
		});

		std::ofstream out;
		out.open(m_fullPath);

		out << tableName << " = " << std::endl;
		out << "{" << std::endl;

		for (auto iter = m_keyValuePair.begin(); iter != m_keyValuePair.end();)
		{
			if (++iter == m_keyValuePair.end())
			{
				out << "    " << iter->first << " = " << iter->second << std::endl;
			}
			else
			{
				out << "    " << iter->first << " = " << iter->second << "," << std::endl;
			}
		}

		out << "}" << std::endl;
		out.close();
	}

	void ConfigReader::setPath(const std::string& path)
	{
		m_path = path;
	}

	bool ConfigReader::parse(const std::string& configFile)
	{
		m_fullPath = m_path + configFile;
		bool success = true;

		// Check if config file exists.
		std::ifstream cf;
		cf.open(m_fullPath);

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

			RE_LOG(LogLevel::WARNING, "Failed to load config file! Creating one...", "ConfigReader::parse", "Config.cpp", 40);

			std::ofstream newFile;
			newFile.open(m_fullPath);

			createEmptyConfig(newFile);

			newFile.close();

			std::ifstream cf2;
			cf2.open(m_fullPath);

			// Retry to load script.
			if (cf2.good())
			{
				// Load script with lua through sol2.
				std::string cfstr((std::istreambuf_iterator<char>(cf2)), std::istreambuf_iterator<char>());

				m_lua.script(cfstr);
			}
			else
			{
				RE_LOG(LogLevel::FATAL, "Failed to create config file", "ConfigReader::parse", "Config.cpp", 62);
				success = false;
			}

			cf2.close();
		}

		return success;
	}

	void ConfigReader::createEmptyConfig(std::ofstream& newFile)
	{
		newFile << "config =" << std::endl;
		newFile << "{" << std::endl;
		newFile << "    appTitle = \"Default\"," << std::endl;
		newFile << "    assetPath = \"bin/assets/\"," << std::endl;
		newFile << "    ups = 60.0," << std::endl;
		newFile << "    versionMajor = 1," << std::endl;
		newFile << "    versionMinor = 0," << std::endl;
		newFile << "    versionPatch = 0," << std::endl;
		newFile << "    screenWidth = 640," << std::endl;
		newFile << "    screenHeight = 480," << std::endl;
		newFile << "    renderingLayers = 2," << std::endl;
		newFile << "    framerateLimit = 0," << std::endl;
		newFile << "    keyRepeat = true," << std::endl;
		newFile << "    cursorVisible = true," << std::endl;
		newFile << "    vsyncEnabled = false," << std::endl;
		newFile << "    saveLog = false," << std::endl;
		newFile << "    enableDebug = false" << std::endl;
		newFile << "}" << std::endl;
	}
}
