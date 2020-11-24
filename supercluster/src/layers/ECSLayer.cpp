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
		m_camera.create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);

		set_name("ecs_layer");
		m_window = SL_HANDLE.window();
		m_world  = SL_HANDLE.world();

		//auto e = m_world->create_from_json("assets/json/entities/demo_entity.json");
	}

	ECSLayer::~ECSLayer()
	{
		m_world  = nullptr;
		m_window = nullptr;
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
		m_camera.update(dt);
		m_world->update(dt);
	}

	void ECSLayer::render()
	{
		m_world->get_system<galaxy::RenderSystem>()->render(m_camera);
	}
} // namespace sc