///
/// Scene3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/graphics/Renderer3D.hpp"
#include "galaxy/res/ShaderBook.hpp"
#include "galaxy/systems/RenderSystem3D.hpp"
#include "galaxy/systems/TransformSystem3D.hpp"

#include "Scene3D.hpp"

namespace galaxy
{
	namespace scene
	{
		Scene3D::Scene3D(std::string_view name) noexcept
		    : Scene {name, "3D"}
		{
			m_dispatcher.subscribe<events::KeyDown>(m_camera);
			m_dispatcher.subscribe<events::KeyUp>(m_camera);
			m_dispatcher.subscribe<events::MouseMoved>(m_camera);
			m_dispatcher.subscribe<events::MouseWheel>(m_camera);
			m_dispatcher.subscribe<events::WindowResized>(m_camera);

			m_world.create_system<systems::TransformSystem3D>();
			m_world.create_system<systems::RenderSystem3D>();
		}

		Scene3D::~Scene3D() noexcept
		{
		}

		void Scene3D::on_push()
		{
			SL_HANDLE.window()->set_scene_dispatcher(&m_dispatcher);
			m_gui.enable_input();
		}

		void Scene3D::on_pop()
		{
			SL_HANDLE.window()->set_scene_dispatcher(nullptr);
			m_gui.disable_input();
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
			if (m_light.is_dirty())
			{
				static constexpr const auto SIZEOF_DIR_LIGHT = sizeof(light::Directional);

				RENDERER_3D().reserve_ssbo(1, SIZEOF_DIR_LIGHT);
				RENDERER_3D().sub_buffer_ssbo(1, 0, 1, &m_light.get_data());
			}

			RENDERER_3D().sub_buffer_ubo(0, 0, 1, &m_camera.get_data());

			RENDERER_3D().forward_render(&m_skybox, SL_HANDLE.shaderbook()->get("skybox"));

			RENDERER_3D().bind_shadowpass();
			m_world.get_system<systems::RenderSystem3D>()->render_shadows(m_world, m_camera);

			RENDERER_3D().bind_geompass();
			m_world.get_system<systems::RenderSystem3D>()->render(m_world, m_camera);

			RENDERER_2D().bind();
			m_gui.render();
		}

		graphics::Camera* Scene3D::get_camera() noexcept
		{
			return static_cast<graphics::Camera*>(&m_camera);
		}

		graphics::Skybox& Scene3D::skybox() noexcept
		{
			return m_skybox;
		}

		graphics::DirectionLight& Scene3D::light() noexcept
		{
			return m_light;
		}

		nlohmann::json Scene3D::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["name"]   = m_name;
			json["type"]   = "3D";
			json["camera"] = m_camera.serialize();
			json["world"]  = m_world.serialize();
			json["theme"]  = m_gui_theme.serialize();
			json["gui"]    = m_gui.serialize();
			json["skybox"] = m_skybox.serialize();
			json["light"]  = m_light.serialize();

			return json;
		}

		void Scene3D::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");

			m_camera.deserialize(json.at("camera"));
			m_world.deserialize(json.at("world"));
			m_skybox.deserialize(json.at("skybox"));
			m_light.deserialize(json.at("light"));

			m_gui_theme.deserialize(json.at("theme"));
			m_gui.set_theme(&m_gui_theme);
			m_gui.deserialize(json.at("gui"));
		}
	} // namespace scene
} // namespace galaxy