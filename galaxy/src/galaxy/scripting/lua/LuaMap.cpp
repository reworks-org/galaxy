///
/// LuaMap.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/map/World.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_map()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			auto map_type        = lua.new_usertype<map::Map>("Map", sol::constructors<map::Map()>());
			map_type["load"]     = &map::Map::load;
			map_type["registry"] = &map::Map::m_registry;
			map_type["name"]     = &map::Map::name;

			auto world_type          = lua.new_usertype<map::World>("World", sol::constructors<map::World()>());
			world_type["file"]       = &map::World::file;
			world_type["get_active"] = &map::World::get_active;
			world_type["load"]       = &map::World::load;
			world_type["name"]       = &map::World::name;
			world_type["parse"]      = &map::World::parse;
			world_type["set_active"] = &map::World::set_active;
		}
	} // namespace lua
} // namespace galaxy
