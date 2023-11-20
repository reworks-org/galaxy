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

		glm::vec2 Random::gen_vec2(const glm::vec2& min, const glm::vec2& max)
		{
			return {gen<float>(min.x, max.x), gen<float>(min.y, max.y)};
		}

		glm::vec3 Random::gen_vec3(const glm::vec3& min, const glm::vec3& max)
		{
			return {gen<float>(min.x, max.x), gen<float>(min.y, max.y), gen<float>(min.z, max.z)};
		}

		glm::vec4 Random::gen_vec4(const glm::vec4& min, const glm::vec4& max)
		{
			return {gen<float>(min.x, max.x), gen<float>(min.y, max.y), gen<float>(min.z, max.z), gen<float>(min.w, max.w)};
		}
	} // namespace math
} // namespace galaxy
