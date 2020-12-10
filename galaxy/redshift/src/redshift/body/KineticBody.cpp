///
/// KineticBody.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include "KineticBody.hpp"

///
/// Core namespace.
///
namespace rs
{
	KineticBody::KineticBody()
	{
	}

	KineticBody::~KineticBody()
	{
	}

	const bool KineticBody::is_rigid() const noexcept
	{
		return false;
	}
} // namespace rs