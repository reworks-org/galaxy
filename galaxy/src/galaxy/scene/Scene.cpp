///
/// Scene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/systems/AnimationSystem.hpp"
#include "galaxy/systems/PhysicsSystem.hpp"
#include "galaxy/systems/ScriptSystem.hpp"

#include "Scene.hpp"

namespace galaxy
{
	namespace scene
	{
		Scene::Scene()
			: m_camera {false}
			, m_world {this}
			, m_context {nullptr}
			, m_map {this}
			, m_name {"Untitled"}
			, m_window {&core::ServiceLocator<core::Window>::ref()}
		{
			m_world.create_system<systems::ScriptSystem>();
			m_world.create_system<systems::AnimationSystem>();
			m_world.create_system<systems::PhysicsSystem>();
			m_world.create_system<systems::RenderSystem>();

			m_dispatcher.sink<events::KeyDown>().connect<&graphics::Camera::on_key_down>(m_camera);
			m_dispatcher.sink<events::MouseWheel>().connect<&graphics::Camera::on_mouse_wheel>(m_camera);
		}

		Scene::~Scene()
		{
		}

		void Scene::load()
		{
		}

		void Scene::unload()
		{
			// Cleanup loaded lua data from entities.
			core::ServiceLocator<sol::state>::ref().collect_garbage();
		}

		void Scene::update()
		{
			m_window->trigger_queued_events(m_dispatcher);
			m_world.update();

			graphics::Renderer::buffer_camera(m_camera);
		}

		void Scene::render()
		{
			graphics::Renderer::draw();
		}

		nlohmann::json Scene::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["camera"]      = m_camera.serialize();
			json["world"]       = m_world.serialize();
			json["name"]        = m_name;
			json["active_map"]  = m_map.get_name();

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			m_camera.deserialize(json.at("camera"));
			m_world.deserialize(json.at("world"));
			m_name = json.at("name");
			m_map.load_map(json.at("active_map"));
		}
	} // namespace scene
} // namespace galaxy