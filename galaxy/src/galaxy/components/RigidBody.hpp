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
			/// Set new box2d collider shape.
			///
			/// \param w Half extant width.
			/// \param h Half extant height.
			///
			void set_shape(const float hw, const float hh) noexcept;

			///
			/// Set body type.
			///
			/// \param type Box2d body enum type.
			///
			void set_type(const b2BodyType type) noexcept;

			///
			/// Set body density.
			///
			/// \param density Affects mass of body.
			///
			void set_density(const float density) noexcept;

			///
			/// Set friction of body.
			///
			/// \param friction Affects movement against other bodies during collision.
			///
			void set_friction(const float friction) noexcept;

			///
			/// Set restitution of body.
			///
			/// \param restitution Affects bounce of body.
			///
			void set_restitution(const float restitution) noexcept;

			///
			/// Set restitution threshold.
			///
			/// \param restitution_threshold Affects bounce threshold of body.
			///
			void set_restitution_threshold(const float restitution_threshold) noexcept;

			///
			/// Set if body is a bullet type.
			///
			/// \param is_bullet Enables continous collision.
			///
			void set_bullet(const bool is_bullet) noexcept;

			///
			/// Set rotation status of body.
			///
			/// \param fixed_rotation Can body rotate.
			///
			void set_fixed_rotation(const bool fixed_rotation) noexcept;

			///
			/// Get body shape.
			///
			/// \return Const glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& get_shape() const noexcept;

			///
			/// Get body type.
			///
			/// \return Box2d enum body type.
			///
			[[nodiscard]] b2BodyType get_type() const noexcept;

			///
			/// Get body density.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_density() const noexcept;

			///
			/// Get body friction.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_friction() const noexcept;

			///
			/// Get body restitution.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_restitution() const noexcept;

			///
			/// Get restitution threshold.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_restitution_threshold() const noexcept;

			///
			///
			///
			/// \return True if continously colliding.
			///
			[[nodiscard]] bool is_bullet() const noexcept;

			///
			/// Get rotation status.
			///
			/// \return True if body can't rotate.
			///
			[[nodiscard]] bool is_rotation_fixed() const noexcept;

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
			/// Can this body rotate?
			///
			bool m_fixed_rotation;

			///
			/// Pointer to Box2D body.
			///
			b2Body* m_body;
		};
	} // namespace components
} // namespace galaxy

#endif