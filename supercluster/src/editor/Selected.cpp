///
/// Selected.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include "Selected.hpp"

namespace sc
{
	Selected::Selected() noexcept
		: m_selected {entt::null}
		, m_world {nullptr}
	{
	}

	Selected::~Selected() noexcept
	{
		m_world    = nullptr;
		m_selected = entt::null;
	}
} // namespace sc