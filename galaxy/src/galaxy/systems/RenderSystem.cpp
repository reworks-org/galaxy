///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Primitive.hpp"
#include "galaxy/components/Flag.hpp"
#include "galaxy/components/Map.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/flags/Enabled.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/scene/Scene.hpp"

#include "RenderSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		RenderSystem::RenderSystem()
		{
		}

		RenderSystem::~RenderSystem()
		{
		}

		void RenderSystem::update(scene::Scene* scene)
		{
			// Possible to do this in parallel but overhead would cost more than perf saved.

			const auto spr_group  = scene->m_world.m_registry.group<components::Sprite>(entt::get<components::Transform, components::Flag>);
			const auto prim_group = scene->m_world.m_registry.group<components::Primitive>(entt::get<components::Transform, components::Flag>);
			const auto text_group = scene->m_world.m_registry.group<components::Text>(entt::get<components::Transform, components::Flag>);
			const auto map_group  = scene->m_world.m_registry.group<components::Map>(entt::get<components::Transform, components::Flag>);

			graphics::RenderCommand cmd;

			for (auto&& [entity, sprite, transform, flag] : spr_group.each())
			{
				if (flag.is_flag_set<flags::Enabled>())
				{
					transform.set_origin(sprite.get_width() * 0.5f, sprite.get_height() * 0.5f);

					cmd.instances              = 1;
					cmd.layer                  = sprite.get_layer();
					cmd.mode                   = graphics::primitive_to_gl(graphics::Primitives::TRIANGLE);
					cmd.texture                = sprite.get_texture_handle();
					cmd.vao                    = sprite.get_vao().id();
					cmd.count                  = sprite.get_vao().index_count();
					cmd.uniform_data.entity    = static_cast<int>(entt::to_integral(entity));
					cmd.uniform_data.colour    = {1.0f, 1.0f, 1.0f, sprite.get_opacity()};
					cmd.uniform_data.transform = transform.get_transform();
					cmd.uniform_data.point     = false;
					cmd.uniform_data.textured  = true;

					graphics::Renderer::submit(cmd);
				}
			}

			for (auto&& [entity, primitive, transform, flag] : prim_group.each())
			{
				if (flag.is_flag_set<flags::Enabled>())
				{
					transform.set_origin(primitive.get_width() * 0.5f, primitive.get_height() * 0.5f);

					cmd.instances              = 1;
					cmd.layer                  = primitive.get_layer();
					cmd.mode                   = primitive.get_mode();
					cmd.texture                = primitive.get_texture_handle();
					cmd.vao                    = primitive.get_vao().id();
					cmd.count                  = primitive.get_vao().index_count();
					cmd.uniform_data.entity    = static_cast<int>(entt::to_integral(entity));
					cmd.uniform_data.colour    = primitive.m_colour.to_vec4();
					cmd.uniform_data.transform = transform.get_transform();
					cmd.uniform_data.point     = primitive.get_shape() == graphics::Shape::POINT;
					cmd.uniform_data.textured  = false;

					graphics::Renderer::submit(cmd);
				}
			}

			for (auto&& [entity, text, transform, flag] : text_group.each())
			{
				if (flag.is_flag_set<flags::Enabled>())
				{
					transform.set_origin(text.get_width() * 0.5f, text.get_height() * 0.5f);

					cmd.instances              = 1;
					cmd.layer                  = text.get_layer();
					cmd.mode                   = graphics::primitive_to_gl(graphics::Primitives::TRIANGLE);
					cmd.texture                = text.get_texture_handle();
					cmd.vao                    = text.get_vao().id();
					cmd.count                  = text.get_vao().index_count();
					cmd.uniform_data.entity    = static_cast<int>(entt::to_integral(entity));
					cmd.uniform_data.colour    = text.m_colour.to_vec4();
					cmd.uniform_data.transform = transform.get_transform();
					cmd.uniform_data.point     = false;
					cmd.uniform_data.textured  = true;

					graphics::Renderer::submit(cmd);
				}
			}

			for (auto&& [entity, map, transform, flag] : map_group.each())
			{
				if (flag.is_flag_set<flags::Enabled>())
				{
					cmd.instances              = 1;
					cmd.layer                  = map.get_layer();
					cmd.mode                   = graphics::primitive_to_gl(graphics::Primitives::TRIANGLE);
					cmd.texture                = map.get_texture_handle();
					cmd.vao                    = map.get_vao().id();
					cmd.count                  = map.get_vao().index_count();
					cmd.uniform_data.entity    = 0; // static_cast<int>(entt::to_integral(entity));
					cmd.uniform_data.colour    = {1.0f, 1.0f, 1.0f, 1.0f};
					cmd.uniform_data.transform = transform.get_transform();
					cmd.uniform_data.point     = false;
					cmd.uniform_data.textured  = true;

					graphics::Renderer::submit(cmd);
				}
			}
		}
	} // namespace systems
} // namespace galaxy