///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include <sol/sol.hpp>

#include "galaxy/core/Command.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "LuaUtils.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	void LuaUtils::writeTable(const sol::table& table, const std::string& file, const std::string& name)
	{
		// Opens a file to write to and prints formatting correctly.
		std::ofstream out(file);
		out << name << " = " << "\n";
		out << "{" << "\n";

		// Loops over each pair in table to write out.
		table.for_each([&](std::pair<sol::object, sol::object> pair)
		{
			out << "    " << pair.first.as<std::string>() << " = " << pair.second.as<std::string>() << "," << "\n";
		});

		// Erase last ',' from table.
		out << '\b';
		out << '\b';
		out << " ";

		// Close up table and flush stream.
		out << "}" << std::endl;
		out.close();
	}

	void LuaUtils::registerUsertypes()
	{
		auto lua = SL_HANDLE.lua();
		lua->new_usertype<galaxy::Command>("Command",
			"exec", &Command::exec,
			"undo", &Command::undo);


	}
}