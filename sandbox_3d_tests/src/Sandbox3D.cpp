///
/// Sandbox3D.cpp
/// Sandbox3D_3d_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>

#include "Sandbox3D.hpp"

using namespace galaxy;

namespace s3d
{
	Sandbox3D::Sandbox3D()
	{
		m_window = SL_HANDLE.window();

		auto scene = m_scene_stack.create<scene::Scene3D>("Sandbox3DScene");
		m_scene_stack.push("Sandbox3DScene");

		scene->m_gui.disable_input();
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