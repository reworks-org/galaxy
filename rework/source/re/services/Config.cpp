//
//  Config.cpp
//  rework
//
//  Created by reworks on 17/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "re/utils/Log.hpp"

#include "Config.hpp"

namespace re
{
	ConfigReader::ConfigReader(const std::string& fileName, std::function<void(std::ofstream&)>& newFile)
	:m_fileName(fileName)
	{
		m_config = al_load_config_file(m_fileName.c_str());

		if (!m_config)
		{
			BOOST_LOG_TRIVIAL(warning) << "Failed to load config file. Creating default..." << std::endl;

			std::ofstream newConfig;
			newConfig.open(m_fileName);

			newFile(newConfig);

			newConfig.close();

			m_config = al_load_config_file(m_fileName.c_str());
			if (!m_config)
			{
				BOOST_LOG_TRIVIAL(error) << "Failed to create and load a config file!" << std::endl;
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

	void ConfigReader::addSection(const std::string& section)
	{
		al_add_config_section(m_config, section.c_str());
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
			BOOST_LOG_TRIVIAL(error) << "Failed to save config file." << std::endl;
		}
	}
}
