///
/// WorldGoesToPhysSystem.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/geometric.hpp>

#include "WorldGoesToPhysSystem.hpp"

///
/// Core namespace.
///
namespace rs
{
	WorldGoesToPhysSystem::WorldGoesToPhysSystem()
	    : m_gravity {0.0f, 9.18f}
	{
	}

	WorldGoesToPhysSystem::WorldGoesToPhysSystem(const glm::vec2& gravity)
	    : m_gravity {gravity}
	{
	}

	WorldGoesToPhysSystem::~WorldGoesToPhysSystem()
	{
	}

	void WorldGoesToPhysSystem::step(const double dt)
	{
		for (auto&& body : m_removed)
		{
			m_collision_tree.insert(body);
		}

		m_removed.clear();

		for (auto&& body : m_bodies)
		{
			auto body_as_collidable = std::static_pointer_cast<Collidable>(body);
			if (!body->is_rigid())
			{
				auto* kin_body   = dynamic_cast<KineticBody*>(body.get());
				const auto force = kin_body->get_force() + m_gravity;

				kin_body->m_velocity += (kin_body->m_mass * force) * static_cast<float>(dt);
				kin_body->m_pos += (kin_body->m_velocity * static_cast<float>(dt));
				kin_body->update_aabb();

				m_collision_tree.update(body_as_collidable);
				kin_body->apply_force(0.0f, 0.0f);
			}

			// Broad Phase collision.
			body_as_collidable = std::static_pointer_cast<Collidable>(body);
			auto possible      = m_collision_tree.query(body_as_collidable);
			for (auto&& obj_b : possible)
			{
				auto* obj_b_as_body = dynamic_cast<Body*>(obj_b.get());
				auto res            = narrow_phase_collision(body.get(), obj_b_as_body);
				if (res != std::nullopt)
				{
					resolve_collision(body.get(), obj_b_as_body, res.value());
				}
			}

			if (!possible.empty())
			{
				m_collision_tree.remove(body_as_collidable);
				m_removed.push_back(body_as_collidable);
			}
		}
	}

	void WorldGoesToPhysSystem::set_gravity(glm::vec2& gravity)
	{
		m_gravity = std::move(gravity);
	}

	std::optional<Manifold> WorldGoesToPhysSystem::narrow_phase_collision(Body* a, Body* b)
	{
		const glm::vec2 a_to_b = b->get_pos() - a->get_pos();
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

	void WorldGoesToPhysSystem::resolve_collision(Body* a, Body* b, const Manifold& manifold)
	{
		float a_inv_mass = a->mass();
		float b_inv_mass = b->mass();
		if (a_inv_mass <= 0)
		{
			a_inv_mass = 0;
		}
		else
		{
			a_inv_mass = 1.0f / a->mass();
		}

		if (b_inv_mass <= 0)
		{
			b_inv_mass = 0;
		}
		else
		{
			b_inv_mass = 1.0f / b->mass();
		}

		if (a->is_rigid())
		{
			auto* b_kin         = dynamic_cast<KineticBody*>(b);
			b_kin->m_velocity.x = 0.0f;
			b_kin->m_velocity.y = 0.0f;
		}
		else if (b->is_rigid())
		{
			auto* a_kin         = dynamic_cast<KineticBody*>(a);
			a_kin->m_velocity.x = 0.0f;
			a_kin->m_velocity.y = 0.0f;
		}
		else
		{
			glm::vec2 rel_vel           = b->get_vel() - a->get_vel();
			const float normal_velocity = glm::dot<2, float, glm::qualifier::highp>(rel_vel, manifold.m_normal);
			if (!(normal_velocity > 0))
			{
				const float restitution = std::min(a->m_restitution, b->m_restitution);
				float impulse_scalar    = -(1.0f + restitution) * normal_velocity;
				impulse_scalar /= a_inv_mass + b_inv_mass;

				const glm::vec2 impulse = impulse_scalar * manifold.m_normal;
				auto* a_kin             = dynamic_cast<KineticBody*>(a);
				a_kin->m_velocity -= a_inv_mass * impulse;

				auto* b_kin = dynamic_cast<KineticBody*>(b);
				b_kin->m_velocity += b_inv_mass * impulse;

				const glm::vec2 correction = std::max(manifold.m_penetration - SLOP_THRESHOLD, 0.0f) / (a_inv_mass + b_inv_mass) * PENETRATION_PERCENT * manifold.m_normal;
				a_kin->m_pos -= a_inv_mass * correction;
				a_kin->update_aabb();

				b_kin->m_pos += b_inv_mass * correction;
				b_kin->update_aabb();

				// Solve friction.
				rel_vel           = b->get_vel() - a->get_vel();
				glm::vec2 tangent = rel_vel - glm::dot<2, float, glm::qualifier::highp>(rel_vel, manifold.m_normal) * manifold.m_normal;
				tangent           = glm::normalize(tangent);

				float friction_magnitude = -glm::dot<2, float, glm::qualifier::highp>(rel_vel, tangent);
				friction_magnitude       = friction_magnitude / (a_inv_mass + b_inv_mass);

				const float mu = std::pow(a->m_static_friction, 2) + std::pow(b->m_static_friction, 2);
				glm::vec2 friction_impulse;
				if (std::abs(friction_magnitude) < impulse_scalar * mu)
				{
					friction_impulse = friction_magnitude * tangent;
				}
				else
				{
					const float dynamic_friction = std::pow(a->m_dynamic_friction, 2) + std::pow(b->m_dynamic_friction, 2);
					friction_impulse             = -impulse_scalar * tangent * dynamic_friction;
				}

				// Apply friction.
				a_kin->m_velocity -= a_inv_mass * friction_impulse;
				b_kin->m_velocity += b_inv_mass * friction_impulse;
			}
		}
	}
} // namespace rs