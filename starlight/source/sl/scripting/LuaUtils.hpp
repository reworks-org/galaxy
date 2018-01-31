///
/// LuaUtils.hpp
/// starlight
///
/// Created by reworks on 02/07/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_LUAUTILS_HPP_
#define STARLIGHT_LUAUTILS_HPP_

#include <map>
#include <string>
#include <fstream>

#include "sol2/sol.hpp"

namespace sl
{
	namespace lua
	{
		///
		/// This writes out a sol::table (or sol::state) into a text file.
		///
		/// \param table sol::table containing information to write out.
		/// \param file The file to write the data into.
		/// \param tableName The name of the table in the text file.
		///
		inline void writeTableToFile(sol::table& table, const std::string& file, const std::string& tableName)
		{
			std::map<std::string, std::string> kvp;
			table.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				kvp.insert({ pair.first.as<std::string>(), pair.second.as<std::string>() });
			});

			std::ofstream out(file);
			out << tableName << " = " << std::endl;
			out << "{" << std::endl;

			for (auto iter = kvp.begin(); iter != kvp.end();)
			{
				if (++iter == kvp.end())
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