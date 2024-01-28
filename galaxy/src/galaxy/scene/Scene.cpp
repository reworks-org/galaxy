///
/// Scene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

#include "galaxy/components/UIScript.hpp"
#include "galaxy/core/Loader.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/resource/Maps.hpp"
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
			, m_map {nullptr}
		{
			init();
		}

		Scene::Scene(const std::string& name)
			: m_camera {false}
			, m_world {this}
			, m_name {name}
			, m_map {nullptr}
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
			load_maps();
		}

		void Scene::unload()
		{
			core::ServiceLocator<sol::state>::ref().collect_garbage();
		}

		void Scene::update()
		{
			auto& window   = core::ServiceLocator<core::Window>::ref();
			auto& renderer = core::ServiceLocator<graphics::Renderer>::ref();

			window.trigger_queued_events(m_dispatcher);
			m_world.update();
			renderer.buffer_camera(m_camera);
		}

		void Scene::render()
		{
			auto& renderer = core::ServiceLocator<graphics::Renderer>::ref();
			auto& nui      = core::ServiceLocator<ui::NuklearUI>::ref();

			renderer.begin_postprocessing();
			renderer.draw();
			renderer.end_postprocessing();
			renderer.prepare_default();
			renderer.clear();
			renderer.render_postprocessing();

			nui.new_frame();
			update_ui();
			nui.render();
		}

		void Scene::load_maps()
		{
			core::ServiceLocator<core::Loader>::ref().load_maps(m_assigned_maps, m_world.m_registry);
		}

		void Scene::set_active_map(const std::string& map)
		{
			if (m_map)
			{
				m_map->disable();
			}

			m_map = !map.empty() ? core::ServiceLocator<resource::Maps>::ref().get(map) : nullptr;

			if (m_map)
			{
				m_map->enable();
			}
		}

		void Scene::update_ui()
		{
			const auto view = m_world.m_registry.view<components::UIScript>(entt::exclude<flags::Disabled>);
			for (auto&& [entity, script] : view.each())
			{
				if (script.m_update.valid())
				{
					script.m_update(script.m_self);
				}
			}
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
			json["maps"]        = m_assigned_maps;
			json["active_map"]  = m_map ? m_map->get_name() : "";

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			m_camera.deserialize(json.at("camera"));
			m_world.deserialize(json.at("world"));
			m_name          = json.at("name");
			m_assigned_maps = json.at("maps");

			set_active_map(json.at("active_map"));
		}
	} // namespace scene
} // namespace galaxy
