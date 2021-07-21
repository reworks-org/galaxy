///
/// CollisionSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/BatchSprite.hpp"
#include "galaxy/components/OnCollision.hpp"
#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/RigidBody.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Transform2D.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/physics/SAT.hpp"
#include "galaxy/resource/ScriptBook.hpp"

#include "CollisionSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		CollisionSystem::CollisionSystem() noexcept
			: m_mtv {0.0f, 0.0f}
		{
		}

		CollisionSystem::~CollisionSystem() noexcept
		{
			m_bvh.clear();
			m_possible.clear();
		}

		void CollisionSystem::update(core::Scene2D* scene, const double dt)
		{
			m_bvh.clear();

			scene->m_world.operate<components::RigidBody, components::Renderable>(
				[&](const ecs::Entity entity, components::RigidBody* body, components::Renderable* renderable) {
					m_bvh.insert(entity, renderable->get_aabb().min(), renderable->get_aabb().max());
				});

			scene->m_world.operate<components::RigidBody, components::Transform2D>(
				[&](const ecs::Entity entity_a, components::RigidBody* body, components::Transform2D* transform) {
					m_possible.clear();
					m_bvh.query(entity_a, std::back_inserter(m_possible));

					for (const auto& entity_b : m_possible)
					{
						m_mtv.x = 0.0f;
						m_mtv.y = 0.0f;

						physics::SAT object_a {scene->m_world, entity_a};
						physics::SAT object_b {scene->m_world, entity_b};

						if (object_a.intersects(object_b, m_mtv))
						{
							if (body->m_type == physics::BodyType::DYNAMIC)
							{
								transform->move(m_mtv.x, m_mtv.y);
							}

							auto collision_a = scene->m_world.get<components::OnCollision>(entity_a);
							if (collision_a)
							{
								SL_HANDLE.scriptbook()->run(collision_a->m_script);
							}
						}
					}
				});
		}
	} // namespace systems
} // namespace galaxy