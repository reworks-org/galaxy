///
/// Lua.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCRIPTING_LUAUTILS_HPP_
#define GALAXY_SCRIPTING_LUAUTILS_HPP_

namespace galaxy
{
	namespace lua
	{
		///
		/// Registers galaxy_engine into Lua.
		///
		void inject_galaxy_into_lua();

		///
		/// Regsiter static engine services into lua.
		///
		void inject_services_into_lua();
	} // namespace lua
} // namespace galaxy

#endif