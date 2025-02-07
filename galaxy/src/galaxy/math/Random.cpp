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
		Random::Random() noexcept
			: m_rd {}
			, m_mt {m_rd()}
		{
		}

		Random::~Random() noexcept
		{
		}
	} // namespace math
} // namespace galaxy
