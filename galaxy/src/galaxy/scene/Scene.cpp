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
#include "galaxy/ui/NuklearUI.hpp"

#include "Scene.hpp"

namespace galaxy
{
	namespace scene
	{
		Scene::Scene()
			: m_camera {false}
			, m_world {this}
			, m_name {"Untitled"}
			, m_window {&core::ServiceLocator<core::Window>::ref()}
			, m_nui {&core::ServiceLocator<ui::NuklearUI>::ref()}
		{
			init();
		}

		Scene::Scene(const std::string& name)
			: m_camera {false}
			, m_world {this}
			, m_name {name}
			, m_window {&core::ServiceLocator<core::Window>::ref()}
			, m_nui {&core::ServiceLocator<ui::NuklearUI>::ref()}
		{
			init();
		}

		Scene::~Scene()
		{
		}

		void Scene::init()
		{
			m_world.create_system<systems::ScriptSystem>();
			m_world.create_system<systems::AnimationSystem>();
			m_world.create_system<systems::PhysicsSystem>();
			m_world.create_system<systems::RenderSystem>();

			m_dispatcher.sink<events::KeyDown>().connect<&graphics::Camera::on_key_down>(m_camera);
			m_dispatcher.sink<events::MouseWheel>().connect<&graphics::Camera::on_mouse_wheel>(m_camera);
			m_dispatcher.sink<events::WindowResized>().connect<&Scene::on_window_resized>(this);
		}

		void Scene::load()
		{
		}

		void Scene::unload()
		{
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
			m_window->begin_postprocessing();
			graphics::Renderer::draw();
			m_window->end_postprocessing();
			m_window->prepare_screen_for_rendering();
			m_window->render_postprocessing();

			m_nui->new_frame();
			m_nui->process_scripts(m_world.m_registry);
			m_nui->render();
		}

		void Scene::on_window_resized(const events::WindowResized& e)
		{
			m_camera.on_window_resized(e);
		}

		nlohmann::json Scene::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["camera"]      = m_camera.serialize();
			json["world"]       = m_world.serialize();
			json["name"]        = m_name;

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			m_camera.deserialize(json.at("camera"));
			m_world.deserialize(json.at("world"));
			m_name = json.at("name");
		}
	} // namespace scene
} // namespace galaxy