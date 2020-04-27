///
/// LuaUtils.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LUAUTILS_HPP_
#define GALAXY_LUAUTILS_HPP_

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Namespace that contains useful functions for interacting with Lua.
	///
	namespace LuaUtils
	{
		///
		/// Write a sol::table to a file.
		///
		/// \param table Table to serialize.
		/// \param file Name to save table to.
		/// \param name Name of the table.
		///
		void writeTable(const sol::table& table, const std::string& file, const std::string& name);

		///
		/// Registers custom library usertypes with sol. Called for you by Application class.
		///
		void registerUsertypes();
	}
}

#endif