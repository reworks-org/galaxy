///
/// LuaRaylib.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include <entt/locator/locator.hpp>

// #include <raylib-lua-sol.hpp>

#include "../Lua.hpp"

namespace galaxy
{
	void Lua::inject_raylib() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();
		// raylib_lua_sol(lua);
	}
} // namespace galaxy
