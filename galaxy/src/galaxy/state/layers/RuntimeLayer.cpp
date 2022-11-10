///
/// RuntimeLayer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/audio/AudioEngine.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/state/Scene.hpp"

#include "galaxy/systems/AnimationSystem.hpp"
#include "galaxy/systems/ScriptSystem.hpp"

#include "RuntimeLayer.hpp"

namespace galaxy
{
	namespace state
	{
		RuntimeLayer::RuntimeLayer(std::string_view name, Scene* scene) noexcept
			: Layer {name, scene}
		{
			m_world.m_dispatcher.sink<events::MouseWheel>().connect<&graphics::Camera::on_mouse_wheel>(scene->get_camera());
			m_world.m_dispatcher.sink<events::WindowResized>().connect<&graphics::Camera::on_window_resized>(scene->get_camera());

			m_world.create_system<systems::ScriptSystem>();
			m_world.create_system<systems::AnimationSystem>();
			m_world.create_system<systems::RenderSystem>();
		}

		RuntimeLayer::~RuntimeLayer() noexcept
		{
		}

		void RuntimeLayer::on_push()
		{
		}

		void RuntimeLayer::on_pop()
		{
			auto& ae = core::ServiceLocator<audio::AudioEngine>::ref();
			ae.stop_all();
		}

		void RuntimeLayer::events()
		{
			core::ServiceLocator<core::Window>::ref().trigger_queued_events(m_world.m_dispatcher);
		}

		void RuntimeLayer::update()
		{
			m_world.update_systems(this);
		}

		void RuntimeLayer::render()
		{
		}

		nlohmann::json RuntimeLayer::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;
			json["type"]        = "Runtime"; // Used by Layers stack.
			json["world"]       = m_world.serialize();

			return json;
		}

		void RuntimeLayer::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");
			m_world.deserialize(json.at("world"));
		}
	} // namespace state
} // namespace galaxy

/*
*
		void MapLayer::add_tiled_project(std::string_view path)
		{
		}

		void MapLayer::parse_tiled_projects()
		{
		}
		void Scene::create_maps(std::string_view path)
		{
			m_maps_path = static_cast<std::string>(path);

			m_maps.clear();
			m_maps.load(m_maps_path);
			if (!m_maps.parse(m_world))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to parse tiled world.");
			}
		}

		void Scene::set_active_map(std::string_view name)
		{
			m_active_map = static_cast<std::string>(name);
		}

		map::Map* Scene::get_map(std::string_view name)
		{
			return m_maps.get_map(name);
		}

		map::Map* Scene::get_active_map()
		{
			return m_maps.get_map(m_active_map);
		}

		nlohmann::json Scene::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["name"]       = m_name;

			json["world"]      = m_world.serialize();
			json["active-map"] = m_active_map;
			json["maps-path"]  = m_maps_path;
			// json["theme"]  = m_gui_theme.serialize();
			// json["gui"]    = m_gui.serialize();

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");


			m_world.deserialize(json.at("world"));

			m_active_map = json.at("active-map");
			m_maps_path  = json.at("maps-path");

			// m_gui_theme.deserialize(json.at("theme"));
			// m_gui.set_theme(&m_gui_theme);
			// m_gui.deserialize(json.at("gui"));
		}
*/