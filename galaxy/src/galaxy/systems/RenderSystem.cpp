///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/World.hpp"
#include "galaxy/graphics/Renderer.hpp"

#include "RenderSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		RenderSystem::RenderSystem()
		    : m_world {nullptr}
		{
			m_world = SL_HANDLE.world();
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

		void RenderSystem::render(graphics::Camera& camera)
		{
			m_world->operate<graphics::Sprite, Shader, EnabledComponent>([&](const sr::Entity entity, graphics::Sprite* sprite, Shader* shader, EnabledComponent* ef) {
				shader->m_shader.bind();
				shader->m_shader.set_uniform("u_cameraProj", camera.get_proj());
				shader->m_shader.set_uniform("u_cameraView", camera.get_transform());

				SL_HANDLE.renderer()->draw_sprite(sprite->m_sprite, shader->m_shader);
			});
		}
	} // namespace systems
} // namespace galaxy