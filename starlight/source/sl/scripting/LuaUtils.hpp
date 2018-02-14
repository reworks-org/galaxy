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

#include "sol2/sol_forward.hpp"

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
		void writeTableToFile(const sol::table& table, const std::string& file, const std::string& tableName);
	}
}

#endif