///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/DrawShader.hpp"
#include "galaxy/components/Primitive.hpp"
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
		}
	} // namespace systems
} // namespace galaxy