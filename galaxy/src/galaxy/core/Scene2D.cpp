///
/// Scene2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/resource/MusicBook.hpp"
#include "galaxy/systems/ActionSystem.hpp"
#include "galaxy/systems/AnimationSystem.hpp"
#include "galaxy/systems/CollisionSystem.hpp"
#include "galaxy/systems/ParticleSystem.hpp"
#include "galaxy/systems/RenderSystem2D.hpp"
#include "galaxy/systems/TransformSystem.hpp"

#include "Scene2D.hpp"

namespace galaxy
{
	namespace core
	{
		Scene2D::Scene2D(std::string_view name) noexcept
			: Serializable {this}
			, m_name {name}
			, m_active_map {""}
			, m_maps_path {""}
		{
			m_camera.set_speed(100.0f);

			m_dispatcher.subscribe<events::KeyDown>(m_camera);
			m_dispatcher.subscribe<events::KeyUp>(m_camera);
			m_dispatcher.subscribe<events::MouseWheel>(m_camera);
			m_dispatcher.subscribe<events::WindowResized>(m_camera);

			m_world.create_system<systems::ActionSystem>();
			m_world.create_system<systems::TransformSystem>();
			m_world.create_system<systems::AnimationSystem>();
			m_world.create_system<systems::CollisionSystem>();
			m_world.create_system<systems::ParticleSystem>();
			m_world.create_system<systems::RenderSystem2D>();

			m_rendersystem = m_world.get_system<systems::RenderSystem2D>();
		}

		Scene2D::~Scene2D() noexcept
		{
		}

		void Scene2D::on_push()
		{
			SL_HANDLE.musicbook()->stop_all();
			SL_HANDLE.lua()->set("galaxy_current_world", &m_world);
		}

		void Scene2D::on_pop()
		{
			SL_HANDLE.musicbook()->stop_all();
		}

		void Scene2D::events()
		{
			SL_HANDLE.window()->trigger_queued_events(m_dispatcher);
		}

		void Scene2D::update()
		{
			m_camera.update();
			m_world.update(this);
		}

		void Scene2D::pre_render()
		{
		}

		void Scene2D::render()
		{
			RENDERER_2D().buffer_camera(m_camera);
			m_rendersystem->render(m_world);
		}

		void Scene2D::create_maps(std::string_view path)
		{
			m_maps_path = static_cast<std::string>(path);

			m_maps.clear();
			m_maps.load(m_maps_path);
			if (!m_maps.parse(m_world))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to parse tiled world.");
			}
		}

		void Scene2D::set_active_map(std::string_view name)
		{
			m_active_map = static_cast<std::string>(name);
		}

		map::Map* Scene2D::get_map(std::string_view name)
		{
			return m_maps.get_map(name);
		}

		map::Map* Scene2D::get_active_map()
		{
			return m_maps.get_map(m_active_map);
		}

		nlohmann::json Scene2D::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["name"]       = m_name;
			json["camera"]     = m_camera.serialize();
			json["world"]      = m_world.serialize();
			json["active-map"] = m_active_map;
			json["maps-path"]  = m_maps_path;
			//json["theme"]  = m_gui_theme.serialize();
			//json["gui"]    = m_gui.serialize();

			return json;
		}

		void Scene2D::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");

			m_camera.deserialize(json.at("camera"));
			m_world.deserialize(json.at("world"));

			m_active_map = json.at("active-map");
			m_maps_path  = json.at("maps-path");

			//m_gui_theme.deserialize(json.at("theme"));
			//m_gui.set_theme(&m_gui_theme);
			//m_gui.deserialize(json.at("gui"));
		}
	} // namespace core
} // namespace galaxy