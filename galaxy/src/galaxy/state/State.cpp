///
/// State.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "State.hpp"

namespace galaxy
{
	State::State(const std::string& name) noexcept
		: m_name {name}
	{
	}

	const std::string& State::name() const noexcept
	{
		return m_name;
	}
} // namespace galaxy
