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

#include <fstream>
#include <string_view>

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
		static inline void writeTableToFile(const sol::table& table, std::string_view file, std::string_view tableName)
		{
			std::ofstream out(file);
			out << tableName << " = " << "\n";
			out << "{" << "\n";

			table.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				out << "    " << pair.first.as<std::string_view>() << " = " << pair.second.as<std::string_view>() << "," << "\n";
			});

			out << "}" << std::endl;
			out.close();
		}
	}
}