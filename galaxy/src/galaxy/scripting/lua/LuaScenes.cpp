///
/// LuaScenes.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/scene/SceneManager.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_scenes()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			auto scene_type                   = lua.new_usertype<scene::Scene>("Scene", sol::no_constructor);
			scene_type["load"]                = &scene::Scene::load;
			scene_type["camera"]              = &scene::Scene::m_camera;
			scene_type["name"]                = &scene::Scene::m_name;
			scene_type["registry"]            = &scene::Scene::m_registry;
			scene_type["velocity_iterations"] = &scene::Scene::m_velocity_iterations;
			scene_type["position_iterations"] = &scene::Scene::m_position_iterations;
			scene_type["unload"]              = &scene::Scene::unload;
			scene_type["update"]              = &scene::Scene::update;
			scene_type["b2world"]             = &scene::Scene::m_b2world;
			scene_type["dispatcher"]          = &scene::Scene::m_dispatcher;
			scene_type["render"]              = &scene::Scene::render;
			scene_type["update_ui"]           = &scene::Scene::update_ui;
			scene_type["load_world"]          = &scene::Scene::load_world;
			scene_type["world"]               = &scene::Scene::m_world;

			auto scenemanager_type        = lua.new_usertype<scene::SceneManager>("SceneManager", sol::no_constructor);
			scenemanager_type["add"]      = &scene::SceneManager::add;
			scenemanager_type["clear"]    = &scene::SceneManager::clear;
			scenemanager_type["empty"]    = &scene::SceneManager::empty;
			scenemanager_type["get"]      = &scene::SceneManager::get;
			scenemanager_type["has"]      = &scene::SceneManager::has;
			scenemanager_type["load_app"] = &scene::SceneManager::load_app;
			scenemanager_type["remove"]   = &scene::SceneManager::remove;
			scenemanager_type["save_app"] = &scene::SceneManager::save_app;
		}
	} // namespace lua
} // namespace galaxy
