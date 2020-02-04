///
/// StateComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "StateComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	StateComponent::StateComponent() noexcept
		:m_alive(true)
	{
	}

	StateComponent::StateComponent(const bool isAlive) noexcept
		:m_alive(isAlive)
	{
	}
}