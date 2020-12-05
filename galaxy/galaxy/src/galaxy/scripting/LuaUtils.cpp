///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/World.hpp"

#include "LuaUtils.hpp"

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
		void register_types()
		{
			auto lua = SL_HANDLE.lua();
		}

		void register_gui()
		{
		}
	} // namespace lua
} // namespace galaxy