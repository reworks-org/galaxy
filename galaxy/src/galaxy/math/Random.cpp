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
		Random::Random()
			: m_rd {}
			, m_mt {m_rd()}
		{
		}

		Random::~Random()
		{
		}

		Vector2 Random::gen_vec2(const Vector2& min, const Vector2& max)
		{
			return {gen<float>(min.x, max.x), gen<float>(min.y, max.y)};
		}

		Vector3 Random::gen_vec3(const Vector3& min, const Vector3& max)
		{
			return {gen<float>(min.x, max.x), gen<float>(min.y, max.y), gen<float>(min.z, max.z)};
		}

		Vector4 Random::gen_vec4(const Vector4& min, const Vector4& max)
		{
			return {gen<float>(min.x, max.x), gen<float>(min.y, max.y), gen<float>(min.z, max.z), gen<float>(min.w, max.w)};
		}
	} // namespace math
} // namespace galaxy
