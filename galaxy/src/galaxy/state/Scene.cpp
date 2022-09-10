///
/// Scene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Renderer.hpp"

#include "Scene.hpp"

namespace galaxy
{
	namespace state
	{
		Scene::Scene() noexcept
			: m_name {"New Scene"}
			, m_layer_stack {this}
		{
		}

		Scene::~Scene()
		{
			unload();
		}

		void Scene::load()
		{
		}

		void Scene::unload()
		{
			// Cleanup loaded lua data from entities.
			auto& lua = core::ServiceLocator<sol::state>::ref();
			lua.collect_garbage();
		}

		void Scene::events()
		{
			m_layer_stack.events();
		}

		void Scene::update()
		{
			m_layer_stack.update();
			graphics::Renderer::buffer_camera(m_camera);
		}

		void Scene::render()
		{
			m_layer_stack.render();
		}

		void Scene::set_name(const std::string& name) noexcept
		{
			m_name = name;
		}

		const std::string& Scene::get_name() const noexcept
		{
			return m_name;
		}

		Layers& Scene::layers() noexcept
		{
			return m_layer_stack;
		}

		graphics::Camera& Scene::get_camera() noexcept
		{
			return m_camera;
		}

		nlohmann::json Scene::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;
			json["stack"]       = m_layer_stack.serialize();
			json["camera"]      = m_camera.serialize();

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			unload();

			m_name = json.at("name");
			m_layer_stack.deserialize(json.at("stack"));
			m_camera.deserialize(json.at("camera"));
		}
	} // namespace state
} // namespace galaxy

/*



			m_dispatcher.sink<events::MouseWheel>().connect<&graphics::Camera::on_mouse_wheel>(m_camera);
			m_dispatcher.sink<events::WindowResized>().connect<&graphics::Camera::on_window_resized>(m_camera);
			m_window = &core::ServiceLocator<core::Window>::ref();

		Scene::Scene(std::string_view name) noexcept
			: Serializable {this}
			, m_name {name}
		{
			m_world.create_system<systems::ActionSystem>();
			m_world.create_system<systems::TransformSystem>();
			m_world.create_system<systems::AnimationSystem>();
			m_world.create_system<systems::CollisionSystem>();
			m_world.create_system<systems::ParticleSystem>();
			m_world.create_system<systems::RenderSystem2D>();

			m_rendersystem = m_world.get_system<systems::RenderSystem2D>();

		}

		Scene::~Scene() noexcept
		{
		}

		void Scene::on_push()
		{
			SL_HANDLE.musicbook()->stop_all();
			SL_HANDLE.lua()->set("galaxy_current_world", &m_world);
		}

		void Scene::on_pop()
		{
			SL_HANDLE.musicbook()->stop_all();
		}

		void Scene::events()
		{
			SL_HANDLE.window()->trigger_queued_events(m_dispatcher);
		}

		void Scene::update()
		{

			m_world.update(this);

		}

		void Scene::pre_render()
		{
		}

		void Scene::render()
		{

			m_rendersystem->render(m_world);
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