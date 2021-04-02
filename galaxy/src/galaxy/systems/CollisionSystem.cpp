///
/// CollisionSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>
#include <optional>

#include "galaxy/components/BatchSprite.hpp"
#include "galaxy/components/OnCollision.hpp"
#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/RigidBody.hpp"
#include "galaxy/components/Sprite2D.hpp"
#include "galaxy/components/Transform2D.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/World.hpp"
#include "galaxy/physics/SATObject.hpp"
#include "galaxy/res/ScriptBook.hpp"

#include "CollisionSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		CollisionSystem::CollisionSystem() noexcept
		    : m_mtv {0.0f, 0.0f}
		{
		}

		void CollisionSystem::update(core::World& world, const double dt)
		{
			m_tree.clear();
			world.operate<components::RigidBody, components::Transform2D>([&](const ecs::Entity entity, components::RigidBody* body, components::Transform2D* transform) {
				const auto& aabb = body->update_aabb(transform->get_pos().x, transform->get_pos().y);
				m_tree.insert(entity, aabb.min(), aabb.max());
			});

			static std::vector<ecs::Entity> possible = {};
			world.operate<components::RigidBody, components::Transform2D>([&](const ecs::Entity entity_a, components::RigidBody* body, components::Transform2D* transform) {
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

						auto collision_a = world.get<components::OnCollision>(entity_a);
						if (collision_a)
						{
							SL_HANDLE.scriptbook()->run(collision_a->m_script);
						}

						auto collision_b = world.get<components::OnCollision>(entity_b);
						if (collision_b)
						{
							SL_HANDLE.scriptbook()->run(collision_b->m_script);
						}
					}
				}
			});
		}

		physics::DynamicTree<ecs::Entity>* CollisionSystem::get_tree() noexcept
		{
			return &m_tree;
		}
	} // namespace systems
} // namespace galaxy