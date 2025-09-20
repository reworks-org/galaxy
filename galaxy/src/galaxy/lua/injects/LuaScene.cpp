///
/// LuaScenes.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/scene/World.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	void add_wrapper(const std::string& key)
	{
	}

	void Lua::inject_scene() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();

		auto scene_type          = lua.new_usertype<Scene>("Scene", sol::no_constructor);
		scene_type["add_system"] = &Scene::add_system;
		scene_type["clear"]      = &Scene::clear;
		scene_type["load"]       = &Scene::on_push;
		scene_type["name"]       = &Scene::name;
		scene_type["unload"]     = &Scene::on_pop;

		auto world_type   = lua.new_usertype<World>("World", sol::no_constructor);
		world_type["add"] = [](World& self, const std::string& key) {
			self.add(key);
		};
		world_type["clear"]   = &World::clear;
		world_type["get"]     = &World::get;
		world_type["has"]     = &World::has;
		world_type["pop"]     = &World::pop;
		world_type["pop_all"] = &World::pop_all;
		world_type["push"]    = &World::push;
		world_type["remove"]  = &World::remove;
		world_type["stack"]   = &World::stack;
		world_type["storage"] = &World::storage;
		world_type["top"]     = &World::top;
	}
} // namespace galaxy
