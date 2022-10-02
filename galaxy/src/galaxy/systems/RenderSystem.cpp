///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/DrawShader.hpp"
#include "galaxy/components/Primitive.hpp"
#include "galaxy/components/Flag.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/flags/Enabled.hpp"
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
			const auto spr_view = layer->world().m_registry.view<components::Sprite, components::DrawShader, components::Transform, components::Flag>();
			for (auto&& [entity, sprite, shader, transform, flag] : spr_view.each())
			{
				if (flag.is_flag_set<flags::Enabled>())
				{
					transform.set_origin(sprite.get_width() * 0.5f, sprite.get_height() * 0.5f);

					sprite.m_shader_sort_id   = shader.m_shader->id();
					sprite.m_configure_shader = [&]() noexcept -> unsigned {
						shader.m_shader->set_uniform("u_transform", transform.get_transform());
						shader.m_shader->set_uniform("u_opacity", sprite.get_opacity());

						return shader.m_shader->id();
					};

					graphics::Renderer::submit(&sprite);
				}
			}

			const auto prim_view = layer->world().m_registry.view<components::Primitive, components::DrawShader, components::Transform, components::Flag>();
			for (auto&& [entity, primitive, shader, transform, flag] : prim_view.each())
			{
				if (flag.is_flag_set<flags::Enabled>())
				{
					transform.set_origin(primitive.get_width() * 0.5f, primitive.get_height() * 0.5f);

					primitive.m_shader_sort_id   = shader.m_shader->id();
					primitive.m_configure_shader = [&]() noexcept -> unsigned {
						shader.m_shader->set_uniform("u_transform", transform.get_transform());
						shader.m_shader->set_uniform("u_colour", primitive.m_colour);

						return shader.m_shader->id();
					};

					graphics::Renderer::submit(&primitive);
				}
			}

			const auto text_view = layer->world().m_registry.view<components::Text, components::DrawShader, components::Transform, components::Flag>();
			for (auto&& [entity, text, shader, transform, flag] : prim_view.each())
			{
				if (flag.is_flag_set<flags::Enabled>())
				{
					transform.set_origin(text.get_width() * 0.5f, text.get_height() * 0.5f);

					text.m_shader_sort_id   = shader.m_shader->id();
					text.m_configure_shader = [&]() noexcept -> unsigned {
						shader.m_shader->set_uniform("u_transform", transform.get_transform());
						shader.m_shader->set_uniform("u_colour", text.m_colour);

						return shader.m_shader->id();
					};

					graphics::Renderer::submit(&text);
				}
			}
		}
	} // namespace systems
} // namespace galaxy