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
#include <galaxy/components/TransformComponent.hpp>
#include <galaxy/flags/EnabledFlag.hpp>
#include <qs/shaders/Sprites.hpp>

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

		/*
		auto entity = m_world->create();
		m_world->add<galaxy::EnabledFlag>(entity);

		auto spr = m_world->add<galaxy::SpriteComponent>(entity);
		spr->load("wall.png");
		spr->create<qs::BufferTypeDynamic>();

		auto tf = m_world->add<galaxy::TransformComponent>(entity);
		tf->m_transform.move(10.0f, 10.0f);

		auto sh = m_world->add<galaxy::ShaderComponent>(entity);
		sh->m_shader.loadFromRaw(qs::s_spriteVS, qs::s_spriteFS);
		*/
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