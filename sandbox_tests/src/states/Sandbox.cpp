///
/// Sandbox.cpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/graphicsWindow.hpp>
#include <galaxy/core/ServiceLocator.hpp>

#include "../layers/SandboxLayer.hpp"
#include "../layers/GUILayer.hpp"

#include "Sandbox.hpp"

namespace sb
{
	Sandbox::Sandbox()
	{
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
	}

	void Sandbox::update(const double dt)
	{
		m_layers.update(dt);
	}

	void Sandbox::pre_render()
	{
		m_layers.pre_render();
	}

	void Sandbox::render()
	{
		m_layers.render();
	}
} // namespace sb