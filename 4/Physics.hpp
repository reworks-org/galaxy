///
/// PhysicsComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PHYSICSCOMPONENT_HPP_
#define GALAXY_PHYSICSCOMPONENT_HPP_

#include <nlohmann/json_fwd.hpp>
#include <redshift/World.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Flag component used to determine if an entity is "alive" or "dead".
	///
	class PhysicsComponent final
	{
	public:
		///
		/// Constructor.
		///
		PhysicsComponent();

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		PhysicsComponent(const nlohmann::json& json);

		///
		/// Copy constructor.
		///
		PhysicsComponent(const PhysicsComponent&) = delete;

		///
		/// Move constructor.
		///
		PhysicsComponent(PhysicsComponent&&);

		///
		/// Copy assignment operator.
		///
		PhysicsComponent& operator=(const PhysicsComponent&) = delete;

		///
		/// Move assignment operator.
		///
		PhysicsComponent& operator=(PhysicsComponent&&);

		///
		/// Destructor.
		///
		~PhysicsComponent();

		///
		/// Physics body.
		///
		std::shared_ptr<rs::Body> m_body;

	private:
		///
		/// Pointer to physics world.
		///
		rs::World* m_world;
	};
} // namespace galaxy
#endif