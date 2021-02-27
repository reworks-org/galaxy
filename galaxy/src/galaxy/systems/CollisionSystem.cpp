///
/// CollisionSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/RigidBody.hpp"
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
				body->set_size({transform->get_origin().x * 2.0f, transform->get_origin().y * 2.0f});
				const auto& aabb = body->update_aabb(transform->get_pos().x, transform->get_pos().y);

				m_tree.insert(entity, aabb.min(), aabb.max());
			});

			std::vector<ecs::Entity> possible;
			possible.reserve(100);
			glm::vec2 mtv = {0.0f, 0.0f};
			world.operate<components::RigidBody, components::Transform>([&](const ecs::Entity entity_a, components::RigidBody* body, components::Transform* transform) {
				possible.clear();
				m_tree.query(entity_a, std::back_inserter(possible));

				for (const auto& entity_b : possible)
				{
					mtv.x = 0.0f;
					mtv.y = 0.0f;
					if (narrow_phase_collision(world, entity_a, entity_b, mtv))
					{
						m_tree.erase(entity_a);
						transform->move(mtv.x, mtv.y);
						m_dispatcher->trigger<events::Collision>(entity_a, entity_b);
					}
				}
			});
		}

		const bool CollisionSystem::narrow_phase_collision(core::World& world, const ecs::Entity a, const ecs::Entity b, glm::vec2& mtv)
		{
			physics::SATObject object_a {world, a};
			physics::SATObject object_b {world, b};

			return object_a.intersects(object_b, mtv);
		}
	} // namespace systems
} // namespace galaxy