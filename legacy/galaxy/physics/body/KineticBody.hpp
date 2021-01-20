///
/// KineticBody.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PHYSICS_BODY_KINETICBODY_HPP_
#define GALAXY_PHYSICS_BODY_KINETICBODY_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/physics/body/Body.hpp"

namespace galaxy
{
	namespace systems
	{
		class PhysicsSystem;
	}

	namespace physics
	{
		///
		/// A specialized body that can move.
		///
		class KineticBody final : public Body
		{
			friend class systems::PhysicsSystem;

		public:
			///
			/// Argument constructor.
			///
			/// \param density Density of this object.
			/// \param size Size of this object.
			///
			KineticBody(const float density, const glm::vec2& size) noexcept;

			///
			/// Argument constructor.
			///
			/// \param density Density of this object.
			/// \param width Width of the object.
			/// \param height Height of the object.
			///
			KineticBody(const float density, const float width, const float height) noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON object to retrieve data from.
			///
			KineticBody(const nlohmann::json& json);

			///
			/// Virtual destructor.
			///
			virtual ~KineticBody() noexcept = default;

			///
			/// Apply a force to this body.
			///
			/// \param x_force Apply a force to this object on the x-axis.
			/// \param y_force Apply a force to this object on the y-axis.
			///
			void apply_force(const float x_force, const float y_force) noexcept;

			///
			/// Apply a force to this body.
			///
			/// \param force Apply a 2D force to this object.
			///
			void apply_force(glm::vec2& force) noexcept;

			///
			/// Apply a force to this body.
			///
			/// \param x_force Apply a force to this object on the x-axis.
			///
			void apply_horizontal_force(const float x_force) noexcept;

			///
			/// Apply a force to this body.
			///
			/// \param y_force Apply a force to this object on the y-axis.
			///
			void apply_vertical_force(const float y_force) noexcept;

			///
			/// Retrieve force being applied to this object.
			///
			/// \return Const glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& get_force() const noexcept;

			///
			/// Get current velocity.
			///
			/// \return Const glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& get_vel() const noexcept override;

			///
			/// Check if this body is rigid or can move.
			///
			/// \return Const bool.
			///
			[[nodiscard]] const bool is_rigid() const noexcept override;

		private:
			///
			/// Deleted default constructor.
			///
			KineticBody() = delete;

		private:
			///
			/// Force currently being applied to this object.
			///
			glm::vec2 m_force;

			///
			/// Velocity of this body.
			///
			glm::vec2 m_velocity;
		};
	} // namespace physics
} // namespace galaxy

#endif