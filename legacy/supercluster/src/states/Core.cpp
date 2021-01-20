///
/// Core.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/graphics/Window.hpp>

#include "../layers/CoreLayer.hpp"

#include "Core.hpp"

namespace sc
{
	Core::Core()
	{
	}

	void Core::on_push()
	{
		m_layers.add<CoreLayer>();
	}

	void Core::on_pop()
	{
		m_layers.clear();
	}

	void Core::events()
	{
		m_layers.events();
	}

	void Core::update(const double dt)
	{
		m_layers.update(dt);
	}

	void Core::pre_render()
	{
		m_layers.pre_render();
	}

	void Core::render()
	{
		m_layers.render();
	}
} // namespace sc