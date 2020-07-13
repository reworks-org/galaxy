///
/// Body.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_BODY_HPP_
#define REDSHIFT_BODY_HPP_

#include <protostar/math/Vector2.hpp>

///
/// Core namespace.
///
namespace rs
{
	///
	/// Parent class for bodies.
	///
	class Body
	{
	public:
		///
		/// Virtual destructor.
		///
		virtual ~Body() noexcept;

		///
		/// Is the body fixed or dynamic.
		///
		virtual const bool isFixed() const noexcept = 0;

		///
		/// Set position.
		///
		/// \param pos Position to set. Does not move by velocity.
		///
		void setPos(const pr::Vector2<double>& pos) noexcept;

		///
		/// Get position.
		///
		/// \return Vec2 of position.
		///
		const pr::Vector2<double>& getPos() const noexcept;

		///
		/// Set mass.
		///
		/// \param Mass of body.
		///
		void setMass(const double mass) noexcept;

		///
		/// Get mass.
		///
		/// \return Get mass of body as a double.
		///
		const double getMass() const noexcept;

	protected:
		///
		/// Constructor.
		///
		Body() noexcept;

	protected:
		///
		/// Mass of body.
		///
		double m_mass;

		///
		/// Position of Body.
		///
		pr::Vector2<double> m_position;
	};
}

#endif