///
/// LuaUtils.hpp
/// starlight
///
/// Created by reworks on 02/07/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include "sl/libs/sol2/sol.hpp"

#include "LuaUtils.hpp"

namespace sl
{
	namespace lua
	{
		void writeTableToFile(const sol::table& table, const std::string& file, const std::string& tableName)
		{
			// Opens a file to write to and prints formatting correctly.
			std::ofstream out(file);
			out << tableName << " = " << "\n";
			out << "{" << "\n";

			// Loops over each pair in table to write out.
			table.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				out << "    " << pair.first.as<std::string>() << " = " << pair.second.as<std::string>() << "," << "\n";
			});

			// Erase last ',' from table.
			out << '\b\b';
			out << " ";

			// Close up table and flush stream.
			out << "}" << std::endl;
			out.close();
		}
	}
}