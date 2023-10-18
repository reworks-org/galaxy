///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Primitive.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/flags/Disabled.hpp"
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
			auto& renderer = core::ServiceLocator<graphics::Renderer>::ref();

			const auto spr_group  = scene->m_world.m_registry.group<components::Sprite>(entt::get<components::Transform>, entt::exclude<flags::Disabled>);
			const auto prim_group = scene->m_world.m_registry.group<components::Primitive>(entt::get<components::Transform>, entt::exclude<flags::Disabled>);
			const auto text_group = scene->m_world.m_registry.group<components::Text>(entt::get<components::Transform>, entt::exclude<flags::Disabled>);

			graphics::RenderCommand cmd;

			for (auto&& [entity, sprite, transform] : spr_group.each())
			{
				transform.set_origin(sprite.get_width() * 0.5f, sprite.get_height() * 0.5f);

				cmd.instances              = 1;
				cmd.mode                   = graphics::primitive_to_gl(graphics::Primitives::TRIANGLE);
				cmd.uniform_data.entity    = static_cast<int>(entt::to_integral(entity));
				cmd.uniform_data.colour    = {1.0f, 1.0f, 1.0f, sprite.get_opacity()};
				cmd.uniform_data.transform = transform.get_transform();
				cmd.uniform_data.point     = false;
				cmd.uniform_data.textured  = true;
				cmd.renderable             = &sprite;

				renderer.submit(cmd);
			}

			for (auto&& [entity, primitive, transform] : prim_group.each())
			{
				transform.set_origin(primitive.get_width() * 0.5f, primitive.get_height() * 0.5f);

				cmd.instances              = 1;
				cmd.mode                   = primitive.get_mode();
				cmd.uniform_data.entity    = static_cast<int>(entt::to_integral(entity));
				cmd.uniform_data.colour    = primitive.m_colour.to_vec4();
				cmd.uniform_data.transform = transform.get_transform();
				cmd.uniform_data.point     = primitive.get_shape() == graphics::Shape::POINT;
				cmd.uniform_data.textured  = false;
				cmd.renderable             = &primitive;

				renderer.submit(cmd);
			}

			for (auto&& [entity, text, transform] : text_group.each())
			{
				transform.set_origin(text.get_width() * 0.5f, text.get_height() * 0.5f);

				cmd.instances              = 1;
				cmd.mode                   = graphics::primitive_to_gl(graphics::Primitives::TRIANGLE);
				cmd.uniform_data.entity    = static_cast<int>(entt::to_integral(entity));
				cmd.uniform_data.colour    = text.m_colour.to_vec4();
				cmd.uniform_data.transform = transform.get_transform();
				cmd.uniform_data.point     = false;
				cmd.uniform_data.textured  = true;
				cmd.renderable             = &text;

				renderer.submit(cmd);
			}
		}
	} // namespace systems
} // namespace galaxy
