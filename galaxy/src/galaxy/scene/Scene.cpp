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
			m_dispatcher.sink<events::WindowResized>().connect<&graphics::Camera::on_window_resized>(m_camera);
			m_dispatcher.sink<events::WindowResized>().connect<&Scene::on_window_resized>(this);

			m_light_ssbo.ambient_light_colour = {0.0, 0.0, 0.0, 0.2};
			m_light_ssbo.resolution           = {m_window->get_widthf(), m_window->get_heightf()};
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

		void Scene::on_window_resized(const events::WindowResized& e)
		{
			m_light_ssbo.resolution.x = static_cast<float>(e.width);
			m_light_ssbo.resolution.y = static_cast<float>(e.height);
		}

		void Scene::update()
		{
			m_window->trigger_queued_events(m_dispatcher);
			m_world.update();

			graphics::Renderer::buffer_camera(m_camera);
			graphics::Renderer::buffer_light_data(m_light_ssbo);
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

			json["ambient_light_colour"]["r"] = m_light_ssbo.ambient_light_colour.x;
			json["ambient_light_colour"]["g"] = m_light_ssbo.ambient_light_colour.y;
			json["ambient_light_colour"]["b"] = m_light_ssbo.ambient_light_colour.z;
			json["ambient_light_colour"]["a"] = m_light_ssbo.ambient_light_colour.w;

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			m_camera.deserialize(json.at("camera"));
			m_world.deserialize(json.at("world"));
			m_name = json.at("name");
			m_map.load_map(json.at("active_map"));

			const auto& ambient_light_colour    = json.at("ambient_light_colour");
			m_light_ssbo.ambient_light_colour.x = ambient_light_colour.at("r");
			m_light_ssbo.ambient_light_colour.y = ambient_light_colour.at("g");
			m_light_ssbo.ambient_light_colour.z = ambient_light_colour.at("b");
			m_light_ssbo.ambient_light_colour.w = ambient_light_colour.at("a");
		}
	} // namespace scene
} // namespace galaxy