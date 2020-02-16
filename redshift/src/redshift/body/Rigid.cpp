///
/// Rigid.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include "Rigid.hpp"

///
/// Core namespace.
///
namespace rs
{
	Rigid::Rigid() noexcept
	{
	}

	Rigid::~Rigid() noexcept
	{
	}

	const bool Rigid::isFixed() const noexcept
	{
		return true;
	}
}