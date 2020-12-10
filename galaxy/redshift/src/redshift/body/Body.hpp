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
	class Body : public rs::Collidable
	{
	public:
		///
		/// Velocity of this body.
		///
		glm::vec2 m_velocity;

		///
		/// Impulse of this body.
		///
		glm::vec2 m_impulse;

		///
		/// Mass of this body.
		///
		float m_mass;

		///
		/// Restitution of this body.
		///
		float m_restitution;

		///
		/// Can this body move.
		///
		/// \return Const bool.
		///
		virtual const bool is_rigid() const noexcept = 0;

	protected:
		///
		/// Constructor.
		///
		Body();

		///
		/// Virtual destructor.
		///
		virtual ~Body() = default;
	};
} // namespace rs

#endif