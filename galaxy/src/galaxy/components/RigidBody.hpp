///
/// RigidBody.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_RIGIDBODY_HPP_
#define GALAXY_COMPONENTS_RIGIDBODY_HPP_

#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <glm/vec2.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/physics/Material.hpp"

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
			/// Set the physics material.
			///
			/// \param material New material to use.
			///
			void set_material(const physics::Material& material) noexcept;

			///
			/// Set new box2d collider shape.
			///
			/// \param w Half extant width.
			/// \param h Half extant height.
			///
			void set_shape(const float hw, const float hh) noexcept;

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

			///
			/// Update internal body fixture.
			///
			void update_fixture() noexcept;

		private:
			///
			/// Can this body rotate?
			///
			bool m_fixed_rotation;

			///
			/// Material of the physics object.
			///
			physics::Material m_material;

			///
			/// Box2D body definition.
			///
			b2BodyDef m_def;

			///
			/// Box2D body shape.
			///
			glm::vec2 m_shape;

			///
			/// Pointer to Box2D body.
			///
			b2Body* m_body;
		};
	} // namespace components
} // namespace galaxy

#endif