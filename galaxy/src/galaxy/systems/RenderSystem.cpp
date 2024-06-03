///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/TileMap.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/scene/Scene.hpp"

#include "RenderSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		template<typename GC>
		void add_cmd(entt::registry& registry)
		{
			const auto group = registry.group<GC>(entt::get<components::Transform>, entt::exclude<flags::Disabled>);

			for (auto&& [entity, gc, tf] : group.each())
			{
				graphics::RenderCommand cmd;

				if constexpr ((std::is_same_v<GC, components::Circle>) || (std::is_same_v<GC, components::Ellipse>) ||
							  (std::is_same_v<GC, components::Point>) || (std::is_same_v<GC, components::Polygon>) ||
							  (std::is_same_v<GC, components::Polyline>))
				{
					graphics::Shape* shape = &gc.m_shape;

					cmd.count             = shape->vao().count();
					cmd.instances         = shape->vao().instances();
					cmd.mode              = shape->mode();
					cmd.offset            = shape->vao().offset();
					cmd.vao               = shape->vao().id();
					cmd.texture           = 0;
					cmd.uniforms.colour   = shape->m_colour.vec4();
					cmd.uniforms.entity   = static_cast<int>(entt::to_integral(entity));
					cmd.uniforms.textured = false;
					cmd.uniforms.point    = shape->mode() == GL_POINTS ? true : false;
				}
				else if constexpr (std::is_same<GC, components::Sprite>::value)
				{
					cmd.count             = gc.m_vao.count();
					cmd.instances         = gc.m_vao.instances();
					cmd.mode              = GL_TRIANGLES;
					cmd.offset            = gc.m_vao.offset();
					cmd.vao               = gc.m_vao.id();
					cmd.texture           = gc.get_texture()->id();
					cmd.uniforms.colour   = gc.m_tint.vec4();
					cmd.uniforms.entity   = static_cast<int>(entt::to_integral(entity));
					cmd.uniforms.textured = true;
					cmd.uniforms.point    = false;
				}
				else if constexpr (std::is_same<GC, components::Text>::value)
				{
					cmd.count             = gc.m_text.vao().count();
					cmd.instances         = gc.m_text.vao().instances();
					cmd.mode              = GL_TRIANGLES;
					cmd.offset            = gc.m_text.vao().offset();
					cmd.vao               = gc.m_text.vao().id();
					cmd.texture           = gc.m_text.render_texture().texture();
					cmd.uniforms.colour   = gc.m_text.m_colour.vec4();
					cmd.uniforms.entity   = static_cast<int>(entt::to_integral(entity));
					cmd.uniforms.textured = true;
					cmd.uniforms.point    = false;
				}
				else if constexpr (std::is_same_v<GC, components::TileMap>)
				{
					cmd.count             = gc.m_batch.vao().count();
					cmd.instances         = 1;
					cmd.layer             = gc.m_render_layer;
					cmd.mode              = GL_TRIANGLES;
					cmd.offset            = gc.m_batch.vao().offset();
					cmd.vao               = gc.m_batch.vao().id();
					cmd.texture           = gc.get_texture()->id();
					cmd.uniforms.colour   = gc.m_tint;
					cmd.uniforms.entity   = static_cast<int>(entt::to_integral(entity));
					cmd.uniforms.textured = true;
					cmd.uniforms.point    = false;
				}

				cmd.uniforms.transform = tf.m_tf.get_transform();
				graphics::Renderer::ref().submit_cmd(cmd);
			}
		}

		template<typename... GC>
		void handle_renderables(entt::registry& registry)
		{
			(add_cmd<GC>(registry), ...);
		}

		RenderSystem::RenderSystem()
		{
		}

		RenderSystem::~RenderSystem()
		{
		}

		void RenderSystem::update(entt::registry& registry)
		{
			handle_renderables<components::Circle,
				components::Ellipse,
				components::Point,
				components::Polygon,
				components::Polyline,
				components::Sprite,
				components::Text,
				components::TileMap>(registry);
		}
	} // namespace systems
} // namespace galaxy
