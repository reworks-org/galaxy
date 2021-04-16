///
/// Sandbox3D.cpp
/// Sandbox3D_3d_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/graphics/Renderer3D.hpp>
#include <galaxy/res/ShaderBook.hpp>

#include "Sandbox3D.hpp"

using namespace galaxy;

namespace s3d
{
	Sandbox3D::Sandbox3D()
	{
		m_window = SL_HANDLE.window();

		m_scene = m_scene_stack.create<scene::Scene3D>("Sandbox3DScene");

		m_scene->camera().m_forward_key = input::Keys::W;
		m_scene->camera().m_back_key    = input::Keys::S;
		m_scene->camera().m_left_key    = input::Keys::A;
		m_scene->camera().m_right_key   = input::Keys::D;
		m_scene->camera().m_up_key      = input::Keys::Q;
		m_scene->camera().m_down_key    = input::Keys::E;

		m_scene->camera().set_mode(graphics::Camera3D::Mode::FREE);
		m_scene->camera().set_position({0.0f, 0.0f, 3.0f});
		m_scene->camera().set_speed(0.5f);

		m_scene->m_gui.disable_input();

		m_scene_stack.push("Sandbox3DScene");

		m_model.load("backpack.obj");
		m_model.create();

		// Point light.
		m_point_light.m_pos                = {0.7f, 0.2f, 2.0f};
		m_point_light.m_ambient_intensity  = glm::vec3 {0.05f};
		m_point_light.m_diffuse_intensity  = glm::vec3 {0.4f};
		m_point_light.m_specular_intensity = glm::vec3 {0.5f};
		m_point_light.set_attunement(light::Point::Ranges::DIST_7);

		m_pl_obj.m_pos = m_point_light.m_pos;
		m_pl_obj.create();

		// Directional light.
		m_dir_light.m_dir                = {-0.2f, -1.0f, -0.3f};
		m_dir_light.m_ambient_intensity  = glm::vec3 {0.05f};
		m_dir_light.m_diffuse_intensity  = glm::vec3 {0.4f};
		m_dir_light.m_specular_intensity = glm::vec3 {0.5f};

		SL_HANDLE.window()->set_window_background({0, 0, 0, 255});
	}

	Sandbox3D::~Sandbox3D()
	{
		m_window = nullptr;
	}

	void Sandbox3D::events()
	{
		if (m_window->key_down(input::Keys::ESC))
		{
			m_window->close();
		}

		if (SL_HANDLE.window()->key_pressed(input::Keys::K))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		m_scene_stack.events();
	}

	void Sandbox3D::update(const double dt)
	{
		m_scene_stack.update(dt);
	}

	void Sandbox3D::pre_render()
	{
		m_scene_stack.pre_render();
	}

	void Sandbox3D::render()
	{
		RENDERER_3D().draw_model(&m_model, &m_point_light, &m_dir_light, m_scene->camera(), SL_HANDLE.shaderbook()->get("phong"));
		RENDERER_3D().draw_light_object(&m_pl_obj, m_scene->camera(), SL_HANDLE.shaderbook()->get("light_object"));

		m_scene_stack.render();
	}
} // namespace s3d