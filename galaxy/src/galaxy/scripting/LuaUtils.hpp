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
	namespace lua
	{
		///
		/// Create custom types in lua.
		///
		void register_types();

		///
		/// Allow gui interaction from lua.
		///
		void register_gui();
	} // namespace lua
} // namespace galaxy

#endif