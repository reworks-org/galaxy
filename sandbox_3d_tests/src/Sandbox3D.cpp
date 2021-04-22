///
/// Sandbox3D.cpp
/// Sandbox3D_3d_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <magic_enum.hpp>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/fs/Config.hpp>
#include <galaxy/graphics/Renderer3D.hpp>
#include <galaxy/res/ShaderBook.hpp>

#include "Sandbox3D.hpp"

using namespace galaxy;

input::Keys parse_key(const std::string& key)
{
	return magic_enum::enum_cast<input::Keys>(key).value();
}

namespace s3d
{
	Sandbox3D::Sandbox3D()
	{
		m_window = SL_HANDLE.window();

		m_scene = m_scene_stack.create<scene::Scene3D>("Sandbox3DScene");

		m_scene->camera().m_forward_key = parse_key(SL_HANDLE.config()->get<std::string>("key-forward"));
		m_scene->camera().m_back_key    = parse_key(SL_HANDLE.config()->get<std::string>("key-back"));
		m_scene->camera().m_left_key    = parse_key(SL_HANDLE.config()->get<std::string>("key-left"));
		m_scene->camera().m_right_key   = parse_key(SL_HANDLE.config()->get<std::string>("key-right"));
		m_scene->camera().m_up_key      = parse_key(SL_HANDLE.config()->get<std::string>("key-freecam-up"));
		m_scene->camera().m_down_key    = parse_key(SL_HANDLE.config()->get<std::string>("key-freecam-down"));

		m_scene->camera().set_mode(graphics::Camera3D::Mode::FREE);
		m_scene->camera().set_position({0.0f, 0.0f, 3.0f});
		m_scene->camera().set_speed(0.5f);

		m_scene->m_gui.disable_input();

		// clang-format off
		// Must be in order: right, left, top, bottom, front, back.
		std::array<std::string, 6> faces = {
			"right.png",
			"left.png",
			"top.png",
			"bottom.png",
			"front.png",
			"back.png"
		};
		// clang-format on

		m_scene->skybox().load(faces);
		SL_HANDLE.window()->set_window_background({0, 0, 0, 255});

		m_scene->m_world.create_from_json("backpack.json");
		m_scene->m_world.create_from_json("dir.json");
		m_scene->m_world.create_from_json("point.json");
		m_scene->m_world.create_from_json("spot.json");
		m_scene_stack.push("Sandbox3DScene");
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
		m_scene_stack.render();
	}
} // namespace s3d