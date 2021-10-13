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
			return m_keymap.contains(entity);
		}

		Set::Set() noexcept
			: m_count {0}
		{
		}
	} // namespace ecs
} // namespace galaxy