//
//  Config.hpp
//  REngine3
//
//  Created by reworks on 17/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_CONFIG_HPP_
#define RENGINE3_CONFIG_HPP_

#include <string>
#include <fstream>
#include <functional>

#include <allegro5/config.h>

#include "re/utility/Utils.hpp"

namespace re
{
	class ConfigReader
	{
	public:
		///
		/// Open a config file.
		///
		/// \param config Path to the config file.
		/// \param newFile A function pointer to a function that contains ofstream code to write a new config file.
		///
		ConfigReader(const std::string& fileName, std::function<void(std::ofstream&)> newFile);

		///
		/// Clean up the config reader.
		///
		~ConfigReader();
		
		///
		/// Set a value in the config.
		///
		/// \param section Section where the value is located.
		/// \param key Key to refer to the value.
		/// \param value Actual value to write.
		///
		template<typename T>
		void setValue(const std::string& section, const std::string& key, T value);

		///
		/// Remove a value from the config.
		///
		/// \param section Section where the value is located.
		/// \param key Key to the value to remove.
		///
		void removeValue(const std::string& section, const std::string& key);

		///
		/// Removes an entire section from the config file. 
		///
		/// \param section to remove 
		///
		void removeSection(const std::string& section);

		///
		/// Saves the config file.
		///
		void save();

		///
		/// Lookup a value in the config file.
		/// 
		/// \param section Section where the value is located.
		/// \param key Key to refer to the value.
		/// 
		/// \return Returns the value converted to T.
		///
		template<typename T>
		T lookup(const std::string& section, const std::string& key);

	private:
		ALLEGRO_CONFIG* m_config;
		std::string m_fileName;
	};

	template<typename T>
	T ConfigReader::lookup(const std::string& section, const std::string& key)
	{  
		return Utils::convertString<T>(al_get_config_value(m_config, section.c_str(), key.c_str()));
	}

	template<typename T>
	void ConfigReader::setValue(const std::string& section, const std::string& key, T value)
	{
		al_set_config_value(m_config, section.c_str(), key.c_str(), std::to_string(value).c_str());
	}
}

#endif