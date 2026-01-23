///
/// LuaScenes.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/scene/Scene.hpp"
#include "galaxy/scene/SceneManager.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	void Lua::inject_scene() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();

		auto scene_type       = lua.new_usertype<Scene>("Scene", sol::no_constructor);
		scene_type["on_push"] = &Scene::on_push;
		scene_type["name"]    = &Scene::name;
		scene_type["on_pop"]  = &Scene::on_pop;
		scene_type["sys_man"] = &Scene::sys_man;
		scene_type["update"]  = &Scene::update;
		scene_type["render"]  = &Scene::render;

		auto sm_type   = lua.new_usertype<SceneManager>("SceneManager", sol::no_constructor);
		sm_type["add"] = [](SceneManager& self, const std::string& key) {
			self.add(key);
		};
		sm_type["clear"]   = &SceneManager::clear;
		sm_type["get"]     = &SceneManager::get;
		sm_type["has"]     = &SceneManager::has;
		sm_type["pop"]     = &SceneManager::pop;
		sm_type["pop_all"] = &SceneManager::pop_all;
		sm_type["push"]    = &SceneManager::push;
		sm_type["remove"]  = &SceneManager::remove;
		sm_type["render"]  = &SceneManager::render;
		sm_type["stack"]   = &SceneManager::stack;
		sm_type["storage"] = &SceneManager::storage;
		sm_type["top"]     = &SceneManager::top;
		sm_type["update"]  = &SceneManager::update;
	}
} // namespace galaxy
