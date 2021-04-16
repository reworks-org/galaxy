///
/// Scene3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"

#include "Scene3D.hpp"

namespace galaxy
{
	namespace scene
	{
		Scene3D::Scene3D(std::string_view name) noexcept
		    : Scene {name}
		{
			m_dispatcher.subscribe<events::KeyDown>(m_camera);
			m_dispatcher.subscribe<events::KeyUp>(m_camera);
			m_dispatcher.subscribe<events::MouseMoved>(m_camera);
			m_dispatcher.subscribe<events::MouseWheel>(m_camera);
			m_dispatcher.subscribe<events::WindowResized>(m_camera);
		}

		Scene3D::~Scene3D() noexcept
		{
		}

		void Scene3D::on_push()
		{
			SL_HANDLE.window()->set_scene_dispatcher(&m_dispatcher);
		}

		void Scene3D::on_pop()
		{
			SL_HANDLE.window()->set_scene_dispatcher(nullptr);
		}

		void Scene3D::events()
		{
		}

		void Scene3D::update(const double dt)
		{
			m_camera.update(dt);
			m_world.update(dt);
			m_gui.update(dt);
		}

		void Scene3D::pre_render()
		{
		}

		void Scene3D::render()
		{
			m_gui.render();
		}

		graphics::Camera3D& Scene3D::camera() noexcept
		{
			return m_camera;
		}

		nlohmann::json Scene3D::serialize()
		{
			nlohmann::json json = "{}"_json;

			/*
						json["name"]  = m_name;
			json["type"]  = "3D";
			json["world"] = m_world.serialize();
			json["theme"] = m_gui_theme.serialize();
			json["gui"]   = m_gui.serialize();
			*/

			return json;
		}

		void Scene3D::deserialize(const nlohmann::json& json)
		{
			/*
			m_name = json.at("name");

			const auto world_json = json.at("world");
			m_world.deserialize(world_json);

			m_gui_theme.deserialize(json.at("theme"));
			m_gui.set_theme(&m_gui_theme);
			m_gui.deserialize(json.at("gui"));
			*/
		}
	} // namespace scene
} // namespace galaxy