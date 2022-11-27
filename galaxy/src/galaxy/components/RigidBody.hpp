///
/// RigidBody.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_RIGIDBODY_HPP_
#define GALAXY_COMPONENTS_RIGIDBODY_HPP_

#include <box2d/b2_body.h>
#include <glm/vec2.hpp>

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace core
	{
		class World;
	} // namespace core

	namespace systems
	{
		class PhysicsSystem;
	} // namespace systems

	namespace components
	{
		///
		/// Box2D physics body.
		///
		class RigidBody final : public fs::Serializable
		{
			friend class core::World;
			friend class systems::PhysicsSystem;

		public:
			///
			/// Constructor.
			///
			RigidBody() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			RigidBody(const nlohmann::json& json) noexcept;

			///
			/// Move constructor.
			///
			RigidBody(RigidBody&&) noexcept;

			///
			/// Move assignment operator.
			///
			RigidBody& operator=(RigidBody&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~RigidBody() noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy assignment operator.
			///
			RigidBody& operator=(const RigidBody&) = delete;

			///
			/// Copy constructor.
			///
			RigidBody(const RigidBody&) = delete;

		private:
			///
			/// Box2D body shape.
			///
			glm::vec2 m_shape;

			///
			/// Body type.
			///
			b2BodyType m_type;

			///
			/// Can this body rotate?
			///
			bool m_fixed_rotation;

			///
			/// How heavy it is in relation to its area.
			///
			float m_density;

			///
			/// How slippery it is.
			///
			float m_friction;

			///
			/// How bouncy the fixture is.
			///
			float m_restitution;

			///
			/// Restitution velocity threshold.
			///
			float m_restitution_threshold;

			///
			/// Is this a bullet? Does continous collision checking.
			///
			bool m_bullet;

			///
			/// Pointer to Box2D body.
			///
			b2Body* m_body;
		};
	} // namespace components
} // namespace galaxy

#endif