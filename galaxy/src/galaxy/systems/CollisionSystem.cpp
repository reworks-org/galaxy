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
		    : m_mtv {0.0f, 0.0f}, m_quadtree {0, {0, 0, 0, 0}}
		{
		}

		CollisionSystem::~CollisionSystem() noexcept
		{
			m_bvh.clear();
			m_possible.clear();
			m_quadtree.clear();
			m_output.clear();
		}

		void CollisionSystem::update(core::Scene2D* scene, const double dt)
		{
			m_quadtree.clear();
			m_output.clear();

			if (!scene->get_active_map())
			{
				m_quadtree.resize(SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height());
			}
			else
			{
				m_quadtree.resize(scene->get_active_map()->get_width(), scene->get_active_map()->get_height());
			}

			scene->m_world.operate<components::RigidBody, components::Renderable>([&](const ecs::Entity entity, components::RigidBody* body, components::Renderable* renderable) {
				m_quadtree.insert({.m_aabb = &renderable->get_aabb(), .m_entity = entity});
			});

			m_quadtree.query({.m_aabb = &scene->m_camera.get_aabb(), .m_entity = 0}, m_output);

			m_bvh.clear();
			for (auto* object : m_output)
			{
				m_bvh.insert(object->m_entity, object->m_aabb->min(), object->m_aabb->max());
			}

			scene->m_world.operate<components::RigidBody, components::Transform2D>([&](const ecs::Entity entity_a, components::RigidBody* body, components::Transform2D* transform) {
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