///
/// World.cpp
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
	World::World() noexcept
		:m_gravity(0.0, 9.81)
	{
	}

	World::World(const double gravity) noexcept
		:m_gravity(0.0, gravity)
	{
	}

	World::~World() noexcept
	{
	}

	void World::process(const double deltaTime) noexcept
	{
		std::condition_variable finished;

		for (rs::Kinetic& body : m_kinetics)
		{
			// Distribute calculations over multiple threads.
			// Don't need to protect with mutex, since function is only reading the shared variables m_gravity and deltaTime.
			m_asyncResults.emplace_back(std::move(std::async(std::launch::async, &rs::Kinetic::move, body, m_gravity, deltaTime)));
		}

		// Make sure each calculation has completed.
		for (auto& future : m_asyncResults)
		{
			future.get();
		}
	}

	void World::setGravity(const double gravity) noexcept
	{
		m_gravity = gravity;
	}
}