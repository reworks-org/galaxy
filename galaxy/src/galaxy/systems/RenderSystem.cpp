///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/DrawShader.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Transform.hpp"

#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/state/Layer.hpp"

#include "RenderSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		RenderSystem::RenderSystem() noexcept
		{
		}

		RenderSystem::~RenderSystem() noexcept
		{
		}

		void RenderSystem::update(state::Layer* layer)
		{
			const auto view = layer->world().m_registry.view<components::Sprite, components::DrawShader, components::Transform>();

			for (auto&& [entity, sprite, shader, transform] : view.each())
			{
				sprite.set_shader(shader.m_shader->id());
				transform.set_origin(sprite.get_width() / 2.0f, sprite.get_height() / 2.0f);

				shader.m_shader->set_uniform("u_transform", transform.get_transform());
				shader.m_shader->set_uniform("u_opacity", sprite.get_opacity());

				graphics::Renderer::submit(&sprite);
			}
		}
	} // namespace systems
} // namespace galaxy