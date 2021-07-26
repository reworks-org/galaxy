///
/// TransformSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/BatchSprite.hpp"
#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform2D.hpp"

#include "TransformSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		TransformSystem::TransformSystem() noexcept
		{
		}

		TransformSystem::~TransformSystem() noexcept
		{
		}

		void TransformSystem::update(core::Scene2D* scene)
		{
			// clang-format off
			scene->m_world.operate<components::Transform2D, components::Renderable>(
				std::execution::par, [&](const ecs::Entity entity, components::Transform2D* transform, components::Renderable* renderable) {
					if (transform->is_dirty())
					{
						// Ordered this way for compiler optimizations.
						// Most-Least common.
						switch (renderable->m_type)
						{
							case graphics::Renderables::BATCHED:
							{
								auto* batchsprite = scene->m_world.get<components::BatchSprite>(entity);

								transform->set_origin(batchsprite->get_region().m_width * 0.5f, batchsprite->get_region().m_height * 0.5f);
								renderable->update_aabb(
									{transform->get_pos().x, transform->get_pos().y, batchsprite->get_region().m_width, batchsprite->get_region().m_height});
							}
							break;

							case graphics::Renderables::SPRITE:
							{
								auto* sprite = scene->m_world.get<components::Sprite>(entity);

								transform->set_origin(sprite->get_width() * 0.5f, sprite->get_height() * 0.5f);
								renderable->update_aabb({transform->get_pos().x,
														 transform->get_pos().y,
														 static_cast<float>(sprite->get_width()),
														 static_cast<float>(sprite->get_height())});
							}
							break;

							case graphics::Renderables::TEXT:
							{
								auto* text = scene->m_world.get<components::Text>(entity);

								transform->set_origin(text->get_width() * 0.5f, text->get_height() * 0.5f);
								renderable->update_aabb({transform->get_pos().x,
														 transform->get_pos().y,
														 static_cast<float>(text->get_width()),
														 static_cast<float>(text->get_height())});
							}
							break;

							case graphics::Renderables::POINT:
							case graphics::Renderables::LINE:
							case graphics::Renderables::LINE_LOOP:
							{
								auto* primitive = scene->m_world.get<components::Primitive2D>(entity);

								renderable->update_aabb({transform->get_pos().x,
														 transform->get_pos().y,
														 static_cast<float>(primitive->get_width()),
														 static_cast<float>(primitive->get_height())});
								if (primitive->get_width() != 0 && primitive->get_height() != 0)
								{
									transform->set_origin(primitive->get_width() * 0.5f, primitive->get_height() * 0.5f);
								}
							}
							break;
						}
					}
				});
			// clang-format on
		}
	} // namespace systems
} // namespace galaxy