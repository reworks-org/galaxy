///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Batch.hpp"
#include "galaxy/components/DrawShader.hpp"
#include "galaxy/components/Primitive.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/components/Text.hpp"

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
			const auto spr_view = layer->world().m_registry.view<components::Sprite, components::DrawShader, components::Transform>();
			for (auto&& [entity, sprite, shader, transform] : spr_view.each())
			{
				sprite.set_shader(shader.m_shader->id());
				transform.set_origin(sprite.get_width() / 2.0f, sprite.get_height() / 2.0f);

				shader.m_shader->set_uniform("u_transform", transform.get_transform());
				shader.m_shader->set_uniform("u_opacity", sprite.get_opacity());

				graphics::Renderer::submit(&sprite);
			}

			const auto prim_view = layer->world().m_registry.view<components::Primitive, components::DrawShader, components::Transform>();
			for (auto&& [entity, primitive, shader, transform] : prim_view.each())
			{
				primitive.set_shader(shader.m_shader->id());
				transform.set_origin(primitive.get_width() / 2.0f, primitive.get_height() / 2.0f);

				shader.m_shader->set_uniform("u_transform", transform.get_transform());
				shader.m_shader->set_uniform("u_colour", primitive.m_colour);

				graphics::Renderer::submit(&primitive);
			}

			const auto text_view = layer->world().m_registry.view<components::Text, components::DrawShader, components::Transform>();
			for (auto&& [entity, text, shader, transform] : prim_view.each())
			{
				text.set_shader(shader.m_shader->id());
				transform.set_origin(text.get_width() / 2.0f, text.get_height() / 2.0f);

				shader.m_shader->set_uniform("u_transform", transform.get_transform());
				shader.m_shader->set_uniform("u_colour", text.m_colour);

				graphics::Renderer::submit(&text);
			}

			const auto batch_view = layer->world().m_registry.view<components::Batch, components::DrawShader, components::Transform>();
			for (auto&& [entity, batch, shader, transform] : batch_view.each())
			{
				batch.set_shader(shader.m_shader->id());
				transform.set_origin(batch.get_width() / 2.0f, batch.get_height() / 2.0f);

				shader.m_shader->set_uniform("u_transform", transform.get_transform());
				shader.m_shader->set_uniform("u_opacity", batch.get_opacity());

				graphics::Renderer::submit(&batch);
			}
		}
	} // namespace systems
} // namespace galaxy