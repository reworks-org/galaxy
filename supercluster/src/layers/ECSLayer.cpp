///
/// ECSLayer.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/systems/RenderSystem.hpp>
#include <galaxy/components/SpriteComponent.hpp>
#include <galaxy/components/ShaderComponent.hpp>
#include <galaxy/flags/EnabledFlag.hpp>

#include "ECSLayer.hpp"

///
/// Core namespace.
///
namespace sc
{
	ECSLayer::ECSLayer()
	{
		set_name("ECSLayer");
		m_window = SL_HANDLE.window();
		m_world  = SL_HANDLE.world();
	}

	ECSLayer::~ECSLayer()
	{
	}

	void ECSLayer::events()
	{
		if (glfwGetKey(m_window->gl_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			m_window->close();
		}

		m_world->events();
	}

	void ECSLayer::update(const double dt)
	{
		m_world->update(dt);
	}

	void ECSLayer::render(qs::Camera& camera)
	{
		m_world->get_system<galaxy::RenderSystem>()->render(camera);
	}
} // namespace sc