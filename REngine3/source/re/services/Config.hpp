//
//  Config.hpp
//  REngine3
//
//  Created by reworks on 17/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_CONFIG_HPP_
#define RENGINE3_CONFIG_HPP_

#include <string>

#include "re/scripting/sol2/sol.hpp"

namespace re
{
	class ConfigReader
	{
	public:
		/*
		* IMPORTS: path - location of config file.
		* EXPORTS: none
		* PURPOSE: Set up path for config reader.
		*/
		void setPath(const std::string& path);

		/*
		* IMPORTS: none
		* EXPORTS: boolean - true means the config file was sucessfully parsed.
		* PURPOSE: To load and parse the config file for the engine. Will create a default file if one is not found.
		*/
		bool parse(const std::string& configFile);

		/*
		* IMPORTS: std::ofstream object
		* EXPORTS: none
		* PURPOSE: Create a custom empty config depending on what this class is used for.
		*/
		virtual void createEmptyConfig(std::ofstream& newFile);

		/*
		* IMPORTS: table name to write.
		* EXPORTS: none
		* PURPOSE: write internal sol::state table to the config file.
		*/
		void writeConfigToFile();

		/*
		* IMPORTS: configValue - The name of the value to retrieve from the lua table in the config file.
		* EXPORTS: T - a static_cast to the appropriate type of value.
		* PURPOSE: To retrieve data from the config file to set up the engine.
		*/
		template<typename T>
		T lookup(const std::string& configValue);

		template<typename T>
		void changeValue(const std::string& name, T value);

	protected:
		sol::state m_lua;
		std::string m_path;
		std::string m_fullPath;
	};

	template<typename T>
	T ConfigReader::lookup(const std::string& configValue)
	{  
		return m_lua.get<T>(configValue);
	}

	template<typename T>
	void ConfigReader::changeValue(const std::string& name, T value)
	{
		m_lua.set(name,value);
	}
}

#endif