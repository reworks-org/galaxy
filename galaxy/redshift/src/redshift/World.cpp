///
/// GUI.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

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

	void World::step()
	{
		for (auto&& body : m_bodies)
		{
			if (!body->is_rigid())
			{
				// TODO: APPLY MOVEMENT FOR KINETIC BODIES
				m_collision_tree.update(std::static_pointer_cast<Collidable>(body));
			}

			// Broad Phase collision.
			auto possible = m_collision_tree.query(std::static_pointer_cast<Collidable>(body));
		}

		// TODO: Narrow Phase collision.
	}

	void World::set_gravity(glm::vec2& gravity)
	{
		m_gravity = std::move(gravity);
	}

	void World::resolve_collision(std::shared_ptr<Body> a, std::shared_ptr<Body> b)
	{
		glm::vec2 relative_velocity;
		//https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331
	}
} // namespace rs