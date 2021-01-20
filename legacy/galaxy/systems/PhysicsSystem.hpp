///
/// PhysicsSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEM_PHYSICSSYSTEM_HPP_
#define GALAXY_SYSTEM_PHYSICSSYSTEM_HPP_

#include <concepts>
#include <optional>

#include "galaxy/ecs/System.hpp"
#include "galaxy/physics/body/KineticBody.hpp"
#include "galaxy/physics/body/StaticBody.hpp"
#include "galaxy/physics/collision/DynamicTree.hpp"
#include "galaxy/physics/collision/Manifold.hpp"

namespace galaxy
{
	class core::World;

	namespace physics
	{
		///
		/// Must be a physics body.
		///
		template<typename Type>
		concept is_body = (std::is_same<Type, StaticBody>::value || std::is_same<Type, KineticBody>::value);

		///
		/// Require that the object can collide.
		///
		template<typename Type>
		concept is_collidable = std::derived_from<Type, Collidable>;
	} // namespace physics

	namespace systems
	{
		///
		/// System that handles rendering of entities with a graphics::Sprite, TransformComponent, etc.
		///
		class PhysicsSystem final : public ecs::System
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
			PhysicsSystem();

			///
			/// Argument constructor.
			///
			/// \param gravity Gravity to apply to this simulation.
			///
			PhysicsSystem(const glm::vec2& gravity);

			///
			/// Destructor.
			///
			virtual ~PhysicsSystem();

			///
			/// Abstract implementation for processing events.
			///
			void events() override;

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param dt DeltaTime from gameloop.
			///
			void update(const double dt) override;

			///
			/// Create a body in the sim.
			///
			/// \param args Body constructor arguments.
			///
			/// \return Shared pointer to body.
			///
			template<physics::is_body DerivedBody, typename... Args>
			[[maybe_unused]] std::shared_ptr<DerivedBody> create(Args&&... args);

			///
			/// Remove body from sim.
			///
			/// \param body Body to remove.
			///
			template<physics::is_body DerivedBody>
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
			[[nodiscard]] std::optional<physics::Manifold> narrow_phase_collision(physics::Body* a, physics::Body* b);

			///
			/// Resolve the collision between two objects.
			///
			void resolve_collision(physics::Body* a, physics::Body* b, const physics::Manifold& manifold);

			///
			/// Pointer to game world.
			///
			core::World* m_world;

			///
			/// Simulation gravity.
			///
			glm::vec2 m_gravity;

			///
			/// List of bodies in the world.
			///
			std::vector<std::shared_ptr<physics::Body>> m_bodies;

			///
			/// List of bodies temp removed from collision tree.
			///
			std::vector<std::shared_ptr<physics::Collidable>> m_removed;

			///
			/// Broad phase dynamic tree for collision detection.
			///
			physics::DynamicTree m_collision_tree;
		};

		template<physics::is_body DerivedBody, typename... Args>
		inline std::shared_ptr<DerivedBody> PhysicsSystem::create(Args&&... args)
		{
			auto body = std::make_shared<DerivedBody>(std::forward<Args>(args)...);

			m_bodies.push_back(std::static_pointer_cast<physics::Body>(body));
			m_collision_tree.insert(std::static_pointer_cast<physics::Collidable>(body));

			return body;
		}

		template<physics::is_body DerivedBody>
		inline void PhysicsSystem::remove(std::shared_ptr<DerivedBody> body)
		{
			// clang-format off
			m_bodies.erase(std::remove_if(m_bodies.begin(), m_bodies.end(), [&](std::shared_ptr<DerivedBody> ptr)
				{
					return ptr == body;
				}),
				m_bodies.end());
			m_collision_tree.remove(std::static_pointer_cast<physics::Collidable>(body));
			// clang-format on
		}
	} // namespace systems
} // namespace galaxy

#endif