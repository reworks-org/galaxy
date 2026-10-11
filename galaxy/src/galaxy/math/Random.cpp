///
/// Random.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Random.hpp"

namespace galaxy
{
	namespace math
	{
		glm::vec2 random_vec2(const glm::vec2& min, const glm::vec2& max) noexcept
		{
			return {random<float>(min.x, max.x), random<float>(min.y, max.y)};
		}

		glm::vec3 random_vec3(const glm::vec3& min, const glm::vec3& max) noexcept
		{
			return {random<float>(min.x, max.x), random<float>(min.y, max.y), random<float>(min.z, max.z)};
		}

		glm::vec4 random_vec4(const glm::vec4& min, const glm::vec4& max) noexcept
		{
			return {random<float>(min.x, max.x), random<float>(min.y, max.y), random<float>(min.z, max.z), random<float>(min.w, max.w)};
		}
	} // namespace math
} // namespace galaxy
