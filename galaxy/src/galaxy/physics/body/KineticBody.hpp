///
/// KineticBody.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_KINETICBODY_HPP_
#define REDSHIFT_KINETICBODY_HPP_

#include "redshift/body/Body.hpp"

///
/// Core namespace.
///
namespace rs
{
	///
	/// A specialized body that can move.
	///
	class KineticBody final : public rs::Body
	{
		friend class World;

	public:
		///
		/// Argument constructor.
		///
		/// \param density Density of this object.
		/// \param size Size of this object.
		///
		KineticBody(const float density, const glm::vec2& size);

		///
		/// Argument constructor.
		///
		/// \param density Density of this object.
		/// \param width Width of the object.
		/// \param height Height of the object.
		///
		KineticBody(const float density, const float width, const float height);

		///
		/// Virtual destructor.
		///
		virtual ~KineticBody() = default;

		///
		/// Apply a force to this body.
		///
		/// \param x_force Apply a force to this object on the x-axis.
		/// \param y_force Apply a force to this object on the y-axis.
		///
		void apply_force(const float x_force, const float y_force);

		///
		/// Apply a force to this body.
		///
		/// \param force Apply a 2D force to this object.
		///
		void apply_force(glm::vec2& force);

		///
		/// Apply a force to this body.
		///
		/// \param x_force Apply a force to this object on the x-axis.
		///
		void apply_horizontal_force(const float x_force);

		///
		/// Apply a force to this body.
		///
		/// \param y_force Apply a force to this object on the y-axis.
		///
		void apply_vertical_force(const float y_force);

		///
		/// Retrieve force being applied to this object.
		///
		/// \return Const glm::vec2 reference.
		///
		[[nodiscard]] const glm::vec2& get_force() const;

		///
		/// Get current velocity.
		///
		/// \return Const glm::vec2 reference.
		///
		[[nodiscard]] const glm::vec2& get_vel() const override;

		///
		/// Check if this body is rigid or can move.
		///
		/// \return Const bool.
		///
		const bool is_rigid() const noexcept override;

	private:
		///
		/// Deleted default constructor.
		///
		KineticBody() = delete;

		///
		/// Force currently being applied to this object.
		///
		glm::vec2 m_force;

		///
		/// Velocity of this body.
		///
		glm::vec2 m_velocity;
	};
} // namespace rs

#endif