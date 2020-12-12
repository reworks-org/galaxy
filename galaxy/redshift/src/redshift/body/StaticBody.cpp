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
	StaticBody::StaticBody(const float density)
	    : Body {density, 0.0f, 0.0f}
	{
	}

	const bool StaticBody::is_rigid() const noexcept
	{
		return true;
	}
} // namespace rs