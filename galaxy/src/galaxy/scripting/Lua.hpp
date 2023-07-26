///
/// Lua.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCRIPTING_LUA_HPP_
#define GALAXY_SCRIPTING_LUA_HPP_

namespace galaxy
{
	namespace lua
	{
		///
		/// Object-orientation for Lua.
		///
		constexpr const auto middleclass =
#include "lua/middleclass.lua"
			;

		///
		/// Lua serializer and pretty printer.
		///
		constexpr const auto serpent =
#include "lua/serpent.lua"
			;

		///
		/// Load some provided external libs into lua.
		///
		void inject_external_modules();

		///
		/// Load the dependencies galaxy uses into lua.
		///
		void inject_dependencies_into_lua();

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