///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include <sol/sol.hpp>

#include "galaxy/scripting/Command.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "LuaUtils.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Namespace that contains useful functions for interacting with Lua.
	///
	namespace Lua
	{
		void registerTypes()
		{
			auto lua = SL_HANDLE.lua();
			lua->new_usertype<galaxy::Command>("Command",
				"exec", &Command::exec,
				"undo", &Command::undo);
		}
	}
}