///
/// Sandbox.cpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Window.hpp>
#include <galaxy/core/ServiceLocator.hpp>

#include "../layers/SandboxLayer.hpp"
#include "../layers/GUILayer.hpp"

#include "Sandbox.hpp"

namespace sb
{
	Sandbox::Sandbox()
	{
		m_camera.create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);
		m_camera.set_speed(5.0f);
	}

	void Sandbox::on_push()
	{
		m_layers.add<SandboxLayer>();
		m_layers.add<GUILayer>();
	}

	void Sandbox::on_pop()
	{
		m_layers.clear();
	}

	void Sandbox::events()
	{
		m_layers.events();

		auto* window = SL_HANDLE.window();
		if (window->key_down(pr::Keys::W))
		{
			m_camera.on_key_down({pr::Keys::W});
		}
		else
		{
			m_camera.on_key_up({pr::Keys::W});
		}

		if (window->key_down(pr::Keys::S) == GLFW_PRESS)
		{
			m_camera.on_key_down({pr::Keys::S});
		}
		else
		{
			m_camera.on_key_up({pr::Keys::S});
		}

		if (window->key_down(pr::Keys::A) == GLFW_PRESS)
		{
			m_camera.on_key_down({pr::Keys::A});
		}
		else
		{
			m_camera.on_key_up({pr::Keys::A});
		}

		if (window->key_down(pr::Keys::D) == GLFW_PRESS)
		{
			m_camera.on_key_down({pr::Keys::D});
		}
		else
		{
			m_camera.on_key_up({pr::Keys::D});
		}
	}

	void Sandbox::update(const double dt)
	{
		m_camera.update(dt);
		m_layers.update(dt);
	}

	void Sandbox::render()
	{
		m_layers.render(m_camera);
	}
} // namespace sb