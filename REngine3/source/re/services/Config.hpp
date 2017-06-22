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
		* IMPORTS: configFile - The name of the configuration file in the VFS.
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
		* IMPORTS: configValue - The name of the value to retrieve from the lua table in the config file.
		* EXPORTS: T - a static_cast to the appropriate type of value.
		* PURPOSE: To retrieve data from the config file to set up the engine.
		*/
		template<typename T>
		T lookup(const std::string& configValue);

	protected:
		sol::state m_lua;
	};

	template<typename T>
	T ConfigReader::lookup(const std::string& configValue)
	{
		// Cast and return value retrieved from table.
		sol::table configTable = m_lua["config"];        
		return configTable.get<T>(configValue);
	}
}

#endif
