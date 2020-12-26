///
/// PhysicsSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PHYSICSSYSTEM_HPP_
#define GALAXY_PHYSICSSYSTEM_HPP_

#include <galaxy/core/World.hpp>
#include <galaxy/system/System.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// System that handles rendering of entities with a SpriteComponent, TransformComponent, etc.
	///
	class PhysicsSystem final : public sr::System
	{
	public:
		///
		/// Constructor.
		///
		PhysicsSystem();

		///
		/// Copy constructor.
		///
		PhysicsSystem(const PhysicsSystem&) = default;

		///
		/// Move constructor.
		///
		PhysicsSystem(PhysicsSystem&&) = default;

		///
		/// Copy assignment operator.
		///
		PhysicsSystem& operator=(const PhysicsSystem&) = default;

		///
		/// Move assignment operator.
		///
		PhysicsSystem& operator=(PhysicsSystem&&) = default;

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

	private:
		///
		/// Game world.
		///
		galaxy::World* m_game_world;
	};
} // namespace galaxy

#endif