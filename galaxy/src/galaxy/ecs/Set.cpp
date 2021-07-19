///
/// Set.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include "Set.hpp"

namespace galaxy
{
	namespace ecs
	{
		const bool Set::has(const Entity entity) noexcept
		{
			return (std::find(m_entities.begin(), m_entities.end(), entity) != m_entities.end());
		}

		Set::Set() noexcept
			: m_count {0}
		{
		}
	} // namespace ecs
} // namespace galaxy