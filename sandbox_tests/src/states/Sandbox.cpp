///
/// Sandbox.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Window.hpp>
#include <galaxy/core/ServiceLocator.hpp>

#include "../layers/SandboxLayer.hpp"

#include "Sandbox.hpp"

namespace sb
{
	Sandbox::Sandbox()
	{
		m_camera.create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);
	}

	void Sandbox::on_push()
	{
		m_layers.add<SandboxLayer>();
	}

	void Sandbox::on_pop()
	{
		m_layers.clear();
	}

	void Sandbox::events()
	{
		m_layers.events();
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