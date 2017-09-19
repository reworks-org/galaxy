//
//  Config.cpp
//  REngine3
//
//  Created by reworks on 17/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "re/utility/Log.hpp"

#include "Config.hpp"

namespace re
{
	ConfigReader::ConfigReader(const std::string& fileName, std::function<void(std::ofstream)> newFile)
		:m_fileName(fileName)
	{
		m_config = al_load_config_file(m_fileName.c_str()));

		if (!m_config)
		{
			RE_LOG(LogLevel::WARNING, "Failed to load config file. Creating...", "ConfigReader::ConfigReader", "Config.cpp", 21);

			std::ofstream newConfig;
			newConfig.open(m_fileName);

			newFile(newConfig);

			newConfig.close();

			m_config = al_load_config_file(m_fileName.c_str()));
			if (!m_config)
			{
				RE_LOG(LogLevel::FATAL, "Failed to create and load a config file!", "ConfigReader::ConfigReader", "Config.cpp", 33);
			}
		}
	}

	ConfigReader::~ConfigReader()
	{
		al_destroy_config(m_config);
	}
	
	void ConfigReader::removeValue(const std::string& section, const std::string& key)
	{
		al_remove_config_key(m_config, section.c_str(), key.c_str());
	}

	void ConfigReader::removeSection(const std::string& section)
	{
		al_remove_config_section(m_config, section.c_str());
	}

	void ConfigReader::save()
	{
		bool saved = al_save_config_file(m_fileName.c_str(), m_config);
		if (!saved)
		{
			RE_LOG(LogLevel::WARNING, "Failed to save config file.", "ConfigReader::save", "Config.cpp", 59);
		}
	}
}
