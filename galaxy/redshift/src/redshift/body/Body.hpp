///
/// Body.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_BODY_HPP_
#define REDSHIFT_BODY_HPP_

#include <glm/vec2.hpp>

#include "redshift/collision/Collidable.hpp"

///
/// Core namespace.
///
namespace rs
{
	///
	/// A physics body that can interact with the world.
	///
	class Body : public rs::Collidable
	{
	public:
		///
		/// Virtual destructor.
		///
		virtual ~Body() = default;

		///
		/// Set initial position of this body.
		///
		/// \param x X axis pos.
		/// \param y Y axis pos.
		///
		void set_pos(const float x, const float y);

		///
		/// Update the internal AABB box.
		///
		void update_aabb();

		///
		/// Get current position.
		///
		/// \return Const glm::vec2 reference.
		///
		[[nodiscard]] const glm::vec2& get_pos() const;

		///
		/// Get current velocity.
		///
		/// \return Const glm::vec2 reference.
		///
		[[nodiscard]] virtual const glm::vec2& get_vel() const = 0;

		///
		/// Get mass.
		///
		/// \return Const float.
		///
		[[nodiscard]] const float mass() const;

		///
		/// Check if this body is rigid or can move.
		///
		/// \return Const bool.
		///
		virtual const bool is_rigid() const noexcept = 0;

		///
		/// Restitution of this body.
		///
		float m_restitution;

		///
		/// Friction of surface when an object is resting on it.
		///
		float m_static_friction;

		///
		/// Friction of surface when an object is moving along it.
		///
		float m_dynamic_friction;

	protected:
		///
		/// Argument constructor.
		///
		/// \param density Density of this object.
		/// \param size Size of this object.
		///
		Body(const float density, const glm::vec2& size);

		///
		/// Argument constructor.
		///
		/// \param density Density of this object.
		/// \param width Width of the object.
		/// \param height Height of the object.
		///
		Body(const float density, const float width, const float height);

		///
		/// Mass of this body.
		///
		float m_mass;

		///
		/// Internal size.
		///
		glm::vec2 m_size;

		///
		/// Current position of the body.
		///
		glm::vec2 m_pos;

	private:
		///
		/// Constructor.
		///
		Body() = delete;
	};
} // namespace rs

#endif