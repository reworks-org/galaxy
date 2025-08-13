///
/// LuaScenes.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/scene/SceneManager.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_scene_control()
		{
			auto& lua = entt::locator<sol::state>::value();

			auto scene_type          = lua.new_usertype<scene::Scene>("Scene", sol::no_constructor);
			scene_type["add_system"] = &scene::Scene::add_system;
			scene_type["load"]       = &scene::Scene::load;
			scene_type["name"]       = &scene::Scene::name;
			scene_type["render"]     = &scene::Scene::render;
			scene_type["unload"]     = &scene::Scene::unload;
			scene_type["update"]     = &scene::Scene::update;

			auto scenemanager_type        = lua.new_usertype<scene::SceneManager>("SceneManager", sol::no_constructor);
			scenemanager_type["clear"]    = &scene::SceneManager::clear;
			scenemanager_type["empty"]    = &scene::SceneManager::empty;
			scenemanager_type["load_app"] = &scene::SceneManager::load_app;
			scenemanager_type["save_app"] = &scene::SceneManager::save_app;
			scenemanager_type["create"]   = &scene::SceneManager::create;
			scenemanager_type["pop"]      = &scene::SceneManager::pop;
			scenemanager_type["pop_all"]  = &scene::SceneManager::pop_all;
			scenemanager_type["push"]     = &scene::SceneManager::push;
			scenemanager_type["render"]   = &scene::SceneManager::render;
			scenemanager_type["set"]      = &scene::SceneManager::set;
			scenemanager_type["top"]      = &scene::SceneManager::top;
			scenemanager_type["update"]   = &scene::SceneManager::update;
		}
	} // namespace lua
} // namespace galaxy
