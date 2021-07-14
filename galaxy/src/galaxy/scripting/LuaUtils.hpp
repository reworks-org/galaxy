///
/// LuaUtils.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCRIPTING_LUAUTILS_HPP_
#define GALAXY_SCRIPTING_LUAUTILS_HPP_

#include <functional>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace lua
	{
		///
		/// Register some of galaxy's loose functions, allowing them to be called from lua.
		///
		void register_functions();
	} // namespace lua
} // namespace galaxy

#endif