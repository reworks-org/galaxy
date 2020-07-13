///
/// Body.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include "Body.hpp"

///
/// Core namespace.
///
namespace rs
{
	Body::Body() noexcept
		:m_mass(0.0), m_position(0.0, 0.0)
	{
	}
	
	Body::~Body() noexcept
	{
	}

	void Body::setPos(const pr::Vector2<double>& pos) noexcept
	{
		m_position = pos;
	}

	const pr::Vector2<double>& Body::getPos() const noexcept
	{
		return m_position;
	}

	void Body::setMass(const double mass) noexcept
	{
		m_mass = mass;
	}

	const double Body::getMass() const noexcept
	{
		return m_mass;
	}
}