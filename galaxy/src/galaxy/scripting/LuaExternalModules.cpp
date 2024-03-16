///
/// LuaExternalModules.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/embedded/MiddleClass.hpp"
#include "galaxy/resource/embedded/Serpent.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_external_modules()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();
			lua.require_script("middleclass", resource::lua_middleclass, true);
			lua.require_script("serpent", resource::lua_serpent, true);
		}
	} // namespace lua
} // namespace galaxy
