///
/// CollisionSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>
#include <optional>

#include "galaxy/components/BatchSprite.hpp"
#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/RigidBody.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/core/World.hpp"
#include "galaxy/events/Collision.hpp"
#include "galaxy/physics/SATObject.hpp"

#include "CollisionSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		CollisionSystem::CollisionSystem(events::Dispatcher* dispatcher)
		    : m_dispatcher {dispatcher}
		{
		}

		void CollisionSystem::update(core::World& world, const double dt)
		{
			m_tree.clear();
			world.operate<components::RigidBody, components::Transform>([&](const ecs::Entity entity, components::RigidBody* body, components::Transform* transform) {
				const auto& aabb = body->update_aabb(transform->get_pos().x, transform->get_pos().y);
				m_tree.insert(entity, aabb.min(), aabb.max());
			});

			std::vector<ecs::Entity> possible;
			world.operate<components::RigidBody, components::Transform>([&](const ecs::Entity entity_a, components::RigidBody* body, components::Transform* transform) {
				possible.clear();
				m_tree.query(entity_a, std::back_inserter(possible));

				for (const auto& entity_b : possible)
				{
					m_mtv.x = 0.0f;
					m_mtv.y = 0.0f;

					physics::SATObject object_a {world, entity_a};
					physics::SATObject object_b {world, entity_b};

					if (object_a.intersects(object_b, m_mtv))
					{
						m_tree.erase(entity_a);
						transform->move(m_mtv.x, m_mtv.y);

						if (m_dispatcher)
						{
							m_dispatcher->trigger<events::Collision>(entity_a, entity_b);
						}
					}
				}
			});
		}
	} // namespace systems
} // namespace galaxy