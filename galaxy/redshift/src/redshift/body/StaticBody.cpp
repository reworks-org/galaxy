///
/// StaticBody.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include "StaticBody.hpp"

///
/// Core namespace.
///
namespace rs
{
	StaticBody::StaticBody()
	{
	}

	StaticBody::~StaticBody()
	{
	}

	const bool StaticBody::is_rigid() const noexcept
	{
		return true;
	}
} // namespace rs