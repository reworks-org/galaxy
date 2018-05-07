///
/// ConfigReader.cpp
/// starlight
///
/// Created by reworks on 17/07/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include "sl/fs/VirtualFS.hpp"
#include "sl/libs/loguru/loguru.hpp"

#include "ConfigReader.hpp"

namespace sl
{
	ConfigReader::ConfigReader(const std::string& config, std::function<void(std::ofstream&)>& newFile)
	{
		ALLEGRO_CONFIG* ptr = al_load_config_file(config.c_str());
		if (!ptr)
		{
			LOG_S(WARNING) << "Failed to load main config file. Creating default...";

			std::ofstream newConfig;
			newConfig.open(config);

			newFile(newConfig);

			newConfig.close();

			ptr = al_load_config_file(config.c_str());
			if (!ptr)
			{
				LOG_S(FATAL) << "Failed to create and load a main config file!";
			}
			else
			{
				m_resourceMap.emplace(entt::HashedString{ config.c_str() }, ptr);
			}
		}
		else
		{
			m_resourceMap.emplace(entt::HashedString{ config.c_str() }, ptr);
		}
	}

	ConfigReader::~ConfigReader()
	{
		for (auto& it : m_resourceMap)
		{
			al_destroy_config(it.second);
		}
	}

	void ConfigReader::add(const std::string& config)
	{
		ALLEGRO_CONFIG* ptr = al_load_config_file(config.c_str());
		if (!ptr)
		{
			LOG_S(WARNING) << "Failed to load config file!";
		}
		else
		{
			m_resourceMap.emplace(entt::HashedString{ config.c_str() }, ptr);
		}
	}
	
	void ConfigReader::removeValue(entt::HashedString config, const std::string& section, const std::string& key)
	{
		al_remove_config_key(m_resourceMap[config], section.c_str(), key.c_str());
	}

	void ConfigReader::addSection(entt::HashedString config, const std::string& section)
	{
		al_add_config_section(m_resourceMap[config], section.c_str());
	}

	void ConfigReader::removeSection(entt::HashedString config, const std::string& section)
	{
		al_remove_config_section(m_resourceMap[config], section.c_str());
	}

	void ConfigReader::save(const std::string& config)
	{
		bool saved = al_save_config_file(config.c_str(), m_resourceMap[entt::HashedString{ config.c_str() }]);
		if (!saved)
		{
			LOG_S(ERROR) << "Failed to save config file: " << config << ".";
		}
	}

	std::vector<std::string> ConfigReader::getSection(const std::string& config, const std::string& section)
	{
		ALLEGRO_CONFIG_ENTRY* entry;
		std::vector<std::string> output;

		const char* first = al_get_first_config_entry(m_resourceMap[entt::HashedString{ config.c_str() }], section.c_str(), &entry);
		output.emplace_back(first);

		while (entry)
		{
			const char* next = al_get_next_config_entry(&entry);
			if (next)
			{
				output.emplace_back(next);
			}
		}

		return output;
	}

	void ConfigReader::clean()
	{
		m_resourceMap.clear();
	}
}