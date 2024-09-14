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

		sf::Vector2f Random::gen_vec2(const sf::Vector2f& min, const sf::Vector2f& max)
		{
			return {gen<float>(min.x, max.x), gen<float>(min.y, max.y)};
		}

		sf::Vector3f Random::gen_vec3(const sf::Vector3f& min, const sf::Vector3f& max)
		{
			return {gen<float>(min.x, max.x), gen<float>(min.y, max.y), gen<float>(min.z, max.z)};
		}
	} // namespace math
} // namespace galaxy
