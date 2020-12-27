///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/World.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/sprite/Sprite.hpp"

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
			m_world->operate<graphics::Sprite, graphics::Shader>([&](const ecs::Entity entity, graphics::Sprite* sprite, graphics::Shader* shader) {
				shader->bind();
				shader->set_uniform("u_cameraProj", camera.get_proj());
				shader->set_uniform("u_cameraView", camera.get_transform());

				SL_HANDLE.renderer()->draw_sprite(*sprite, *shader);
			});
		}
	} // namespace systems
} // namespace galaxy