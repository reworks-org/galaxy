///
/// ConfigReader.cpp
/// starlight
///
/// Created by reworks on 17/07/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include <allegro5/error.h>

#include "ConfigReader.hpp"

namespace sl
{
	ConfigReader::ConfigReader(const std::string& config, std::function<void(std::ofstream&)>& newFile)
	{
		// Load file and check to make sure file is valid.
		ALLEGRO_CONFIG* ptr = al_load_config_file(config.c_str());
		if (!ptr)
		{
			// If it fails, attempt to create a default file.
			LOG_S(WARNING) << "Failed to load main config file. Attempting to create default. Errno: " << al_get_errno();

			std::ofstream newConfig(config);
			if (!newConfig)
			{
				LOG_S(FATAL) << "Failed to open stream to write to! " << config;
			}
			else
			{
				newFile(newConfig);

				ptr = al_load_config_file(config.c_str());
				if (!ptr)
				{
					// If that fails, fatal error occurs.
					LOG_S(FATAL) << "Failed to create and load a main config file! " << config << "Errno: " << al_get_errno();
				}
				else
				{
					m_resourceMap.emplace(entt::HashedString(config.c_str()), ptr);
				}
			}

			newConfig.close();
			
		}
		else
		{
			m_resourceMap.emplace(entt::HashedString(config.c_str()), ptr);
		}
	}

	ConfigReader::~ConfigReader()
	{
		// See clean().
		clean();
	}

	bool ConfigReader::add(const std::string& config)
	{
		bool result = true;

		// Emplace, making sure file is valid.
		ALLEGRO_CONFIG* ptr = al_load_config_file(config.c_str());
		if (!ptr)
		{
			LOG_S(ERROR) << "Failed to load config file: " << config << "Errno: " << al_get_errno();
			result = false;
		}
		else
		{
			m_resourceMap.emplace(entt::HashedString(config.c_str()), ptr);
		}

		return result;
	}
	
	void ConfigReader::removeValue(const std::string& config, const std::string& section, const std::string& key)
	{
		al_remove_config_key(m_resourceMap[entt::HashedString(config.c_str())], section.c_str(), key.c_str());
	}

	void ConfigReader::addSection(const std::string& config, const std::string& section)
	{
		al_add_config_section(m_resourceMap[entt::HashedString(config.c_str())], section.c_str());
	}

	void ConfigReader::removeSection(const std::string& config, const std::string& section)
	{
		al_remove_config_section(m_resourceMap[entt::HashedString(config.c_str())], section.c_str());
	}

	bool ConfigReader::save(const std::string& config)
	{
		bool result = true;

		if (!al_save_config_file(config.c_str(), m_resourceMap[entt::HashedString(config.c_str())]))
		{
			LOG_S(ERROR) << "Failed to save config file: " << config << "Errno: " << al_get_errno();
			result = false;
		}

		return result;
	}

	std::vector<std::string> ConfigReader::getSection(const std::string& config, const std::string& section)
	{
		ALLEGRO_CONFIG_ENTRY* entry = nullptr;
		std::vector<std::string> output;

		const char* first = al_get_first_config_entry(m_resourceMap[entt::HashedString(config.c_str())], section.c_str(), &entry);
		if (!first)
		{
			LOG_S(ERROR) << "Failed to get first entry: " << config << " section: " << section << " Errno: " << al_get_errno();
		}
		else
		{
			output.emplace_back(first);

			// Iterate over each config entry and add sections to vector.
			while (entry)
			{
				const char* next = al_get_next_config_entry(&entry);
				if (next)
				{
					output.emplace_back(next);
				}
			}
		}

		return output;
	}

	void ConfigReader::clean()
	{
		// Ensure resources are properly cleaned up.
		for (auto& it : m_resourceMap)
		{
			if (it.second)
			{
				al_destroy_config(it.second);
			}
		}

		m_resourceMap.clear();
	}
}