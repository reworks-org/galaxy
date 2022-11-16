///
/// LuaExternalLibs.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCRIPTING_LUAEXTERNALLIBS_HPP_
#define GALAXY_SCRIPTING_LUAEXTERNALLIBS_HPP_

//clang-format off

namespace galaxy
{
	namespace scripting
	{
		///
		/// Human-readable representation of Lua tables.
		///
		constexpr const auto inspect =
#include "lua/inspect.lua"
			;

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
	} // namespace scripting
} // namespace galaxy

//clang-format on

#endif