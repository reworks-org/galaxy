///
///  ConfigReader.hpp
///  rework
///
///  Created by reworks on 17/07/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_CONFIGREADER_HPP_
#define REWORK_CONFIGREADER_HPP_

#include <functional>
#include <allegro5/config.h>

#include "re/utils/Utils.hpp"
#include "re/types/ServiceLocator.hpp"
#include "entt/core/hashed_string.hpp"

namespace re
{
	class ConfigReader : public ServiceLocator<ConfigReader>
	{
	public:
		///
		/// Open a config file.
		///
		/// \param config Path to the main config file. This config file is special because it can be created if missing. If using add, does not create a default.
		/// \param newFile A function pointer to a function that contains ofstream code to write a default config file.
		///
		ConfigReader(const std::string& config, std::function<void(std::ofstream&)>& newFile);

		///
		/// Clean up the config reader.
		///
		~ConfigReader() override;

		///
		/// \brief Add another config to reader.
		///
		/// Does not create a default config.
		///
		void add(const std::string& config);
		
		///
		/// Set a value in the config.
		///
		/// \param section Section where the value is located.
		/// \param key Key to refer to the value.
		/// \param value Actual value to write.
		///
		template<typename T>
		void setValue(entt::HashedString config, const std::string& section, const std::string& key, T value);

		///
		/// Remove a value from the config.
		///
		/// \param section Section where the value is located.
		/// \param key Key to the value to remove.
		///
		void removeValue(entt::HashedString config, const std::string& section, const std::string& key);

		///
		/// Add a section to the config file.
		///
		/// \param section Section to add.
		///
		void addSection(entt::HashedString config, const std::string& section);

		///
		/// Removes an entire section from the config file. 
		///
		/// \param section Section to remove 
		///
		void removeSection(entt::HashedString config, const std::string& section);

		///
		/// Saves the config file.
		///
		void save(const std::string& config);

		///
		/// Lookup a value in the config file.
		/// 
		/// \param section Section where the value is located.
		/// \param key Key to refer to the value.
		/// 
		/// \return Returns the value converted to T.
		///
		template<typename T>
		T lookup(entt::HashedString config, const std::string& section, const std::string& key);

	private:
		std::unordered_map<entt::HashedString::hash_type, ALLEGRO_CONFIG*> m_configs;

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		ConfigReader() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		ConfigReader(const ConfigReader&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		ConfigReader(ConfigReader&&) = delete;
	};

	template<typename T>
	T ConfigReader::lookup(entt::HashedString config, const std::string& section, const std::string& key)
	{  
		return utils::convertString<T>(al_get_config_value(m_configs[config], section.c_str(), key.c_str()));
	}

	template<typename T>
	void ConfigReader::setValue(entt::HashedString config, const std::string& section, const std::string& key, T value)
	{
		al_set_config_value(m_configs[config], section.c_str(), key.c_str(), std::to_string(value).c_str());
	}
}

#endif