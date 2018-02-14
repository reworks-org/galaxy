///
/// LuaUtils.hpp
/// starlight
///
/// Created by reworks on 02/07/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include "sol2/sol.hpp"

#include "LuaUtils.hpp"

namespace sl
{
	namespace lua
	{
		void writeTableToFile(const sol::table& table, const std::string& file, const std::string& tableName)
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