///
/// World.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_WORLD_HPP_
#define REDSHIFT_WORLD_HPP_

#include <optional>

#include "redshift/Meta.hpp"
#include "redshift/collision/DynamicTree.hpp"
#include "redshift/collision/Manifold.hpp"

///
/// Core namespace.
///
namespace rs
{
	///
	/// Physics world simulation.
	///
	class World final
	{
	public:
		///
		/// Controls how far objects are allowed to penetrate during collision.
		///
		static inline const constexpr float PENETRATION_PERCENT = 0.2f;

		///
		/// Threshold for preventing jittering.
		///
		static inline const constexpr float SLOP_THRESHOLD = 0.01f;

		///
		/// Constructor.
		///
		World();

		///
		/// Argument constructor.
		///
		/// \param gravity Gravity to apply to this simulation.
		///
		World(const glm::vec2& gravity);

		///
		/// Copy constructor.
		///
		World(const World&) = default;

		///
		/// Move constructor.
		///
		World(World&&) = default;

		///
		/// Copy assignment operator.
		///
		World& operator=(const World&) = default;

		///
		/// Move assignment operator.
		///
		World& operator=(World&&) = default;

		///
		/// Destructor.
		///
		~World();

		///
		/// Update (step) all physics bodies and process collisions.
		///
		/// \param dt DeltaTime i.e. gameloop lag.
		///
		void step(const double dt);

		///
		/// Create a body in the sim.
		///
		/// \param args Body constructor arguments.
		///
		/// \return Shared pointer to body.
		///
		template<is_body DerivedBody, typename... Args>
		[[maybe_unused]] std::shared_ptr<DerivedBody> create(Args&&... args);

		///
		/// Remove body from sim.
		///
		/// \param body Body to remove.
		///
		template<is_body DerivedBody>
		void remove(std::shared_ptr<DerivedBody> body);

		///
		/// Change the gravity.
		///
		/// \param gravity New gravity to apply.
		///
		void set_gravity(glm::vec2& gravity);

	private:
		///
		/// Perform edge detection for broad phase collision objects.
		///
		[[nodiscard]] std::optional<Manifold> narrow_phase_collision(Body* a, Body* b);

		///
		/// Resolve the collision between two objects.
		///
		void resolve_collision(Body* a, Body* b, const Manifold& manifold);

		///
		/// Simulation gravity.
		///
		glm::vec2 m_gravity;

		///
		/// List of bodies in the world.
		///
		std::vector<std::shared_ptr<Body>> m_bodies;

		///
		/// List of bodies temp removed from collision tree.
		///
		std::vector<std::shared_ptr<Collidable>> m_removed;

		///
		/// Broad phase dynamic tree for collision detection.
		///
		DynamicTree m_collision_tree;
	};

	template<is_body DerivedBody, typename... Args>
	inline std::shared_ptr<DerivedBody> World::create(Args&&... args)
	{
		auto body = std::make_shared<DerivedBody>(std::forward<Args>(args)...);

		m_bodies.push_back(std::static_pointer_cast<Body>(body));
		m_collision_tree.insert(std::static_pointer_cast<Collidable>(body));

		return body;
	}

	template<is_body DerivedBody>
	inline void World::remove(std::shared_ptr<DerivedBody> body)
	{
		// clang-format off
		m_bodies.erase(std::remove_if(m_bodies.begin(), m_bodies.end(), [&](std::shared_ptr<DerivedBody> ptr)
		{
			return ptr == body;
		}),
		m_bodies.end());
		m_collision_tree.remove(std::static_pointer_cast<Collidable>(body));
		// clang-format on
	}
} // namespace rs

#endif