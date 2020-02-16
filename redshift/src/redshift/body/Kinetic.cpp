///
/// Kinetic.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include <cmath>

#include "Kinetic.hpp"

///
/// Core namespace.
///
namespace rs
{
	Kinetic::Kinetic() noexcept
	{
	}

	Kinetic::~Kinetic() noexcept
	{
	}
	
	const bool Kinetic::isFixed() const noexcept
	{
		return false;
	}

	void Kinetic::move(const protostar::Vector2<double>& acc, const double time) noexcept
	{
		// V = U + AT
		m_velocity.m_x = (m_velocity.m_x + (acc.m_x * time));
		m_velocity.m_y = (m_velocity.m_y + (acc.m_y * time));

		// S = VT - 0.5AT^2
		m_position.m_x = (m_velocity.m_x * time) - (0.5 * acc.m_x * std::pow(time, 2));
		m_position.m_y = (m_velocity.m_y * time) - (0.5 * acc.m_y * std::pow(time, 2));
	}
}