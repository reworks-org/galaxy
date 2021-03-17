///
/// Scene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/systems/AnimationSystem.hpp"
#include "galaxy/systems/CollisionSystem.hpp"
#include "galaxy/systems/RenderSystem.hpp"
#include "galaxy/systems/TransformSystem.hpp"

#include "Scene.hpp"

namespace galaxy
{
	namespace core
	{
		Scene::Scene(std::string_view name) noexcept
		    : Serializable {this}, m_name {name}
		{
			m_camera.create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);
			m_camera.set_speed(100.0f);

			m_dispatcher.subscribe<events::KeyDown>(m_camera);
			m_dispatcher.subscribe<events::KeyUp>(m_camera);
			m_dispatcher.subscribe<events::MouseWheel>(m_camera);
			m_dispatcher.subscribe<events::WindowResized>(m_camera);

			m_world.create_system<systems::AnimationSystem>();
			m_world.create_system<systems::TransformSystem>();
			m_world.create_system<systems::RenderSystem>();
			m_world.create_system<systems::CollisionSystem>();

			m_gui_theme.m_camera.create(0.0f, (float)SL_HANDLE.window()->get_width(), (float)SL_HANDLE.window()->get_height(), 0.0f);
			m_gui.set_theme(&m_gui_theme);

			m_dispatcher.subscribe<events::MouseMoved>(m_gui);
			m_dispatcher.subscribe<events::MousePressed>(m_gui);
			m_dispatcher.subscribe<events::MouseReleased>(m_gui);
			m_dispatcher.subscribe<events::KeyDown>(m_gui);
		}

		Scene::~Scene() noexcept
		{
		}

		void Scene::on_push()
		{
			SL_HANDLE.window()->set_scene_dispatcher(&m_dispatcher);
		}

		void Scene::on_pop()
		{
			SL_HANDLE.window()->set_scene_dispatcher(nullptr);
		}

		void Scene::events()
		{
		}

		void Scene::update(const double dt)
		{
			m_camera.update(dt);
			m_world.update(dt);
			m_gui.update(dt);
		}

		void Scene::pre_render()
		{
		}

		void Scene::render()
		{
			m_world.get_system<systems::RenderSystem>()->render(m_world, m_camera);
			m_gui.render();
		}

		nlohmann::json Scene::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["name"]   = m_name;
			json["camera"] = m_camera.serialize();
			json["world"]  = m_world.serialize();
			json["theme"]  = m_gui_theme.serialize();
			json["gui"]    = m_gui.serialize();

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");

			const auto camera_json = json.at("camera");
			const auto world_json  = json.at("world");

			m_camera.deserialize(camera_json);
			m_world.deserialize(world_json);

			m_gui_theme.deserialize(json.at("theme"));
			m_gui.set_theme(&m_gui_theme);
			m_gui.deserialize(json.at("gui"));
		}
	} // namespace core
} // namespace galaxy