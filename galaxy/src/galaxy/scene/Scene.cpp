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
#include "galaxy/systems/RMLSystem.hpp"

#include "Scene.hpp"
#include <RmlUi/Core/Core.h>

namespace galaxy
{
	namespace scene
	{
		Scene::Scene()
			: m_camera {false}
			, m_world {this}
			, m_context {nullptr}
			, m_name {"Untitled"}
			, m_window {&core::ServiceLocator<core::Window>::ref()}
		{
			m_world.create_system<systems::ScriptSystem>();
			m_world.create_system<systems::RMLSystem>();
			m_world.create_system<systems::AnimationSystem>();
			m_world.create_system<systems::PhysicsSystem>();
			m_world.create_system<systems::RenderSystem>();

			m_dispatcher.sink<events::KeyDown>().connect<&graphics::Camera::on_key_down>(m_camera);
			m_dispatcher.sink<events::MouseWheel>().connect<&graphics::Camera::on_mouse_wheel>(m_camera);
			m_dispatcher.sink<events::WindowResized>().connect<&graphics::Camera::on_window_resized>(m_camera);
			m_dispatcher.sink<events::WindowResized>().connect<&Scene::on_window_resized>(this);

			m_lighting.ambient_light_colour = {0.0, 0.0, 0.0, 0.2};
			m_lighting.resolution           = {m_window->get_widthf(), m_window->get_heightf()};

			m_context = Rml::CreateContext("GalaxyScene" + m_name, Rml::Vector2i(m_window->get_width(), m_window->get_height()));
			m_rml_events.set_context(m_context);
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
			m_context->Update();
			m_world.update();

			graphics::Renderer::buffer_camera(m_camera);
			graphics::Renderer::buffer_light_data(m_lighting);
		}

		void Scene::render()
		{
			graphics::Renderer::draw();
		}

		void Scene::on_window_resized(const events::WindowResized& e)
		{
			m_lighting.resolution.x = static_cast<float>(e.width);
			m_lighting.resolution.y = static_cast<float>(e.height);
		}

		nlohmann::json Scene::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["camera"]      = m_camera.serialize();
			json["world"]       = m_world.serialize();
			json["name"]        = m_name;

			json["ambient_light_colour"]["r"] = m_lighting.ambient_light_colour.x;
			json["ambient_light_colour"]["g"] = m_lighting.ambient_light_colour.y;
			json["ambient_light_colour"]["b"] = m_lighting.ambient_light_colour.z;
			json["ambient_light_colour"]["a"] = m_lighting.ambient_light_colour.w;

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			m_camera.deserialize(json.at("camera"));
			m_world.deserialize(json.at("world"));
			m_name = json.at("name");

			const auto& ambient_light_colour  = json.at("ambient_light_colour");
			m_lighting.ambient_light_colour.x = ambient_light_colour.at("r");
			m_lighting.ambient_light_colour.y = ambient_light_colour.at("g");
			m_lighting.ambient_light_colour.z = ambient_light_colour.at("b");
			m_lighting.ambient_light_colour.w = ambient_light_colour.at("a");
		}
	} // namespace scene
} // namespace galaxy