///
/// StaticBody.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "StaticBody.hpp"

namespace galaxy
{
	namespace physics
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
			static const constexpr glm::vec2 s_vel = {0.0f, 0.0f};
			return s_vel;
		}

		const bool StaticBody::is_rigid() const
		{
			return true;
		}
	} // namespace physics
} // namespace galaxy