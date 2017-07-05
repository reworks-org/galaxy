//
//  LuaUtils.hpp
//  REngine3
//
//  Created by reworks on 02/07/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_LUAUTILS_HPP_
#define RENGINE3_LUAUTILS_HPP_

#include <string>
#include <fstream>

#include "sol2/sol.hpp"

namespace re
{
	namespace lua
	{
		/*
		* IMPORTS: table to write out, file to write to, name of table
		* EXPORTS: none
		* PURPOSE: To write out a table to a file
		*/
		inline void writeTableToFile(sol::table& table, const std::string& filePath, const std::string& tableName)
		{
			// Get key-value pairs from table
			std::map<std::string, std::string> m_keyValuePair;
			table.for_each([&](std::pair<sol::object, sol::object> pair) {
				m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<std::string>() });
			});

			std::ofstream out;
			out.open(filePath);

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
	}
}
