///
/// ECSLayer.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/systems/RenderSystem.hpp>
#include <galaxy/components/All.hpp>
#include <galaxy/flags/EnabledFlag.hpp>

#include "ECSLayer.hpp"

///
/// Core namespace.
///
namespace sc
{
	ECSLayer::ECSLayer()
	{
		set_name("ecs_layer");

		m_window = SL_HANDLE.window();
		m_world  = SL_HANDLE.world();

		m_camera.create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);
	}

	ECSLayer::~ECSLayer()
	{
		m_world  = nullptr;
		m_window = nullptr;
	}

	void ECSLayer::events()
	{
		if (m_window->key_pressed(pr::Keys::ESC))
		{
			m_window->close();
		}

		m_world->events();
	}

	void ECSLayer::update(const double dt)
	{
		m_camera.update(dt);
		m_world->update(dt);
	}

	void ECSLayer::render()
	{
	}
} // namespace sc