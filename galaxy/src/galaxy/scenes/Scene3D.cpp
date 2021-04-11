///
/// Scene3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/graphics/Renderer3D.hpp"
#include "galaxy/res/ShaderBook.hpp"

#include "Scene3D.hpp"

namespace galaxy
{
	namespace scene
	{
		Scene3D::Scene3D(std::string_view name) noexcept
		    : Scene {name}
		{
			m_camera.m_forward_key = input::Keys::W;
			m_camera.m_back_key    = input::Keys::S;
			m_camera.m_left_key    = input::Keys::A;
			m_camera.m_right_key   = input::Keys::D;
			m_camera.m_up_key      = input::Keys::Q;
			m_camera.m_down_key    = input::Keys::E;

			m_camera.set_mode(graphics::Camera3D::Mode::FREE);
			m_camera.set_position({0.0f, 0.0f, 3.0f});
			m_camera.set_speed(0.5f);

			m_dispatcher.subscribe<events::KeyDown>(m_camera);
			m_dispatcher.subscribe<events::KeyUp>(m_camera);
			m_dispatcher.subscribe<events::MouseMoved>(m_camera);
			m_dispatcher.subscribe<events::MouseWheel>(m_camera);
			m_dispatcher.subscribe<events::WindowResized>(m_camera);

			m_model.load("backpack.obj");
			m_model.create();

			//https://learnopengl.com/Model-Loading/Model
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
			if (SL_HANDLE.window()->key_pressed(input::Keys::K))
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
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
			RENDERER_3D().draw_model(&m_model, m_camera, SL_HANDLE.shaderbook()->get("model"));

			m_gui.render();
		}

		nlohmann::json Scene3D::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["name"]  = m_name;
			json["type"]  = "3D";
			json["world"] = m_world.serialize();
			json["theme"] = m_gui_theme.serialize();
			json["gui"]   = m_gui.serialize();

			return json;
		}

		void Scene3D::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");

			const auto world_json = json.at("world");
			m_world.deserialize(world_json);

			m_gui_theme.deserialize(json.at("theme"));
			m_gui.set_theme(&m_gui_theme);
			m_gui.deserialize(json.at("gui"));
		}
	} // namespace scene
} // namespace galaxy