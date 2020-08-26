///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Renderer.hpp>

#include "galaxy/components/All.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/World.hpp"
#include "galaxy/flags/EnabledFlag.hpp"

#include "RenderSystem.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	RenderSystem::RenderSystem()
	    : m_world {nullptr}
	{
	}

	RenderSystem::RenderSystem(const RenderSystem& rs)
	{
		this->m_world = rs.m_world;
	}

	RenderSystem::RenderSystem(RenderSystem&& rs)
	{
		this->m_world = rs.m_world;
		rs.m_world    = nullptr;
	}

	RenderSystem& RenderSystem::operator=(const RenderSystem& rs)
	{
		this->m_world = rs.m_world;
		return *this;
	}

	RenderSystem& RenderSystem::operator=(RenderSystem&& rs)
	{
		if (this != &rs)
		{
			this->m_world = rs.m_world;
			rs.m_world    = nullptr;
		}

		return *this;
	}

	RenderSystem::~RenderSystem()
	{
		m_world = nullptr;
	}

	void RenderSystem::events()
	{
	}

	void RenderSystem::update(const double dt)
	{
	}

	void RenderSystem::render(qs::Camera& camera)
	{
		m_world->operate<SpriteComponent, ShaderComponent>([&](const sr::Entity entity, SpriteComponent* sprite, ShaderComponent* shader) {
			shader->m_shader.bind();
			shader->m_shader.set_uniform<glm::mat4>("u_cameraProj", camera.get_proj());
			shader->m_shader.set_uniform<glm::mat4>("u_cameraView", camera.get_transform());

			SL_HANDLE.renderer()->draw_sprite(sprite->m_sprite, shader->m_shader);
		});
	}
} // namespace galaxy