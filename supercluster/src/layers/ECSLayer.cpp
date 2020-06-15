///
/// ECSLayer.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/systems/RenderSystem.hpp>

#include "ECSLayer.hpp"

///
/// Core namespace.
///
namespace sc
{
	ECSLayer::ECSLayer() noexcept
	{
		setName("ECSLayer");
		m_window = SL_HANDLE.window();
		m_world = SL_HANDLE.world();

		m_camera.create(0.0f, m_window->getWidth(), m_window->getHeight(), 0.0f);
	}

	ECSLayer::~ECSLayer() noexcept
	{
	}

	void ECSLayer::events() noexcept
	{
		if (glfwGetKey(m_window->getGLWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			m_window->close();
		}

		m_world->events();
	}

	void ECSLayer::update(protostar::ProtectedDouble* deltaTime) noexcept
	{
		m_world->update(deltaTime);
		m_camera.update(deltaTime->get());
	}

	void ECSLayer::render() noexcept
	{
		m_world->get<galaxy::RenderSystem>()->render(&m_camera);
	}
}