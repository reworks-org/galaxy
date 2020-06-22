///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/World.hpp>
#include <galaxy/core/ServiceLocator.hpp>

#include "galaxy/components/SpriteComponent.hpp"
#include "galaxy/components/TransformComponent.hpp"
#include "galaxy/components/ShaderComponent.hpp"
#include "galaxy/flags/EnabledFlag.hpp"

#include "RenderSystem.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	RenderSystem::RenderSystem() noexcept
	{
	}

	RenderSystem::~RenderSystem() noexcept
	{
	}

	void RenderSystem::events() noexcept
	{
	}

	void RenderSystem::update(protostar::ProtectedDouble* deltaTime) noexcept
	{
	}

	void RenderSystem::render(qs::Camera* camera)
	{
		SL_HANDLE.world()->operate<SpriteComponent, TransformComponent, ShaderComponent, EnabledFlag>([&](sr::Entity, SpriteComponent* sprite, TransformComponent* transform, ShaderComponent* shader, EnabledFlag* ef)
		{
				shader->m_shader.bind();
				sprite->bind();

				shader->m_shader.setUniform("u_transform", transform->m_transform.getTransformation());
				shader->m_shader.setUniform("u_opacity", sprite->getOpacity());
				shader->m_shader.setUniform<float>("u_width", static_cast<float>(sprite->getWidth()));
				shader->m_shader.setUniform<float>("u_height", static_cast<float>(sprite->getHeight()));
				shader->m_shader.setUniform<glm::mat4>("u_cameraProj", camera->getProj());
				shader->m_shader.setUniform<glm::mat4>("u_cameraView", camera->getTransformation());

				glDrawElements(GL_TRIANGLES, sprite->getCount(), GL_UNSIGNED_INT, nullptr);
		});
	}
}