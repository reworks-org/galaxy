///
/// World.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/geometric.hpp>

#include "World.hpp"

///
/// Core namespace.
///
namespace rs
{
	World::World()
	    : m_gravity {0.0f, 9.18f}
	{
	}

	World::World(const glm::vec2& gravity)
	    : m_gravity {gravity}
	{
	}

	World::~World()
	{
	}

	void World::step(const double dt)
	{
		for (auto&& body : m_bodies)
		{
			auto body_as_collidable = std::static_pointer_cast<Collidable>(body);
			if (!body->is_rigid())
			{
				const auto* kin_body = dynamic_cast<KineticBody*>(body.get());

				auto force = kin_body->get_force() + m_gravity;

				body->m_velocity += (1.0f / body->m_mass * force) * static_cast<float>(dt);
				body->m_pos += (body->m_velocity * static_cast<float>(dt));

				m_collision_tree.update(body_as_collidable);
			}

			// Broad Phase collision.
			auto possible = m_collision_tree.query(body_as_collidable);
			for (auto&& obj_b : possible)
			{
				auto* obj_b_as_body = dynamic_cast<Body*>(obj_b.get());
				auto res            = narrow_phase_collision(body.get(), obj_b_as_body);
				if (res != std::nullopt)
				{
					m_collision_tree.remove(body);
					m_collision_tree.remove(obj_b);
					resolve_collision(body.get(), obj_b_as_body, res.value());
				}
			}
		}
	}

	void World::set_gravity(glm::vec2& gravity)
	{
		m_gravity = std::move(gravity);
	}

	std::optional<Manifold> World::narrow_phase_collision(Body* a, Body* b)
	{
		const glm::vec2 a_to_b = b->m_pos - a->m_pos;
		const AABB& a_box      = a->get_aabb();
		const AABB& b_box      = b->get_aabb();

		const float x_a_extent = (a_box.m_max.x - a_box.m_min.x) / 2.0f;
		const float x_b_extent = (b_box.m_max.x - b_box.m_min.x) / 2.0f;
		const float x_overlap  = x_a_extent + x_b_extent - std::abs(a_to_b.x);

		if (x_overlap > 0)
		{
			const float y_a_extent = (a_box.m_max.y - a_box.m_min.y) / 2.0f;
			const float y_b_extent = (b_box.m_max.y - b_box.m_min.y) / 2.0f;
			const float y_overlap  = y_a_extent + y_b_extent - std::abs(a_to_b.y);

			Manifold manifold;

			if (y_overlap > 0)
			{
				if (x_overlap > y_overlap)
				{
					if (a_to_b.x < 0)
					{
						manifold.m_normal = {-1.0f, 0.0f};
					}
					else
					{
						manifold.m_normal = {0.0f, 0.0f};
					}

					manifold.m_penetration = x_overlap;
					return std::make_optional(manifold);
				}
				else
				{
					if (a_to_b.y < 0)
					{
						manifold.m_normal = {0.0f, -1.0f};
					}
					else
					{
						manifold.m_normal = {0.0f, 1.0f};
					}

					manifold.m_penetration = y_overlap;
					return std::make_optional(manifold);
				}
			}
		}

		return std::nullopt;
	}

	void World::resolve_collision(Body* a, Body* b, const Manifold& manifold)
	{
		const glm::vec2 rel_vel     = b->m_velocity - a->m_velocity;
		const float normal_velocity = glm::dot(rel_vel, manifold.m_normal);
		if (!(normal_velocity > 0))
		{
			const float restitution = std::min(a->m_restitution, b->m_restitution);
			float impulse_scalar    = -(1 + restitution) * normal_velocity;
			impulse_scalar /= 1 / a->m_mass + 1 / b->m_mass;

			const glm::vec2 impulse = impulse_scalar * manifold.m_normal;
			a->m_velocity -= 1 / a->m_mass * impulse;
			b->m_velocity += 1 / b->m_mass * impulse;

			float a_inv_mass = a->m_mass;
			float b_inv_mass = b->m_mass;
			if (a_inv_mass <= 0)
			{
				a_inv_mass = 0;
			}
			else
			{
				a_inv_mass = 1.0f / a->m_mass;
			}

			if (b_inv_mass <= 0)
			{
				b_inv_mass = 0;
			}
			else
			{
				b_inv_mass = 1.0f / b->m_mass;
			}

			const glm::vec2 correction = std::max(manifold.m_penetration - SLOP_THRESHOLD, 0.0f) / (a_inv_mass + b_inv_mass) * PENETRATION_PERCENT * (b->m_pos - a->m_pos);
			a->m_pos -= a_inv_mass * correction;
			b->m_pos += b_inv_mass * correction;
		}
	}
} // namespace rs