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
	StaticBody::StaticBody(const float density, const glm::vec2& size)
	    : Body {density, size}
	{
	}

	StaticBody::StaticBody(const float density, const float width, const float height)
	    : Body {density, width, height}
	{
	}

	const glm::vec2& StaticBody::get_vel() const
	{
		const constexpr glm::vec2 static_vel = {0.0f, 0.0f};
		return static_vel;
	}

	const bool StaticBody::is_rigid() const noexcept
	{
		return true;
	}
} // namespace rs