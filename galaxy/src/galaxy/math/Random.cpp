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
		ray::Vector2 random_vec2(const ray::Vector2& min, const ray::Vector2& max) noexcept
		{
			return {random<float>(min.x, max.x), random<float>(min.y, max.y)};
		}

		ray::Vector3 random_vec3(const ray::Vector3& min, const ray::Vector3& max) noexcept
		{
			return {random<float>(min.x, max.x), random<float>(min.y, max.y), random<float>(min.z, max.z)};
		}

		ray::Vector4 random_vec4(const ray::Vector4& min, const ray::Vector4& max) noexcept
		{
			return {random<float>(min.x, max.x), random<float>(min.y, max.y), random<float>(min.z, max.z), random<float>(min.w, max.w)};
		}
	} // namespace math
} // namespace galaxy
