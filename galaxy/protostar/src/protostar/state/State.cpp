///
/// State.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "State.hpp"

namespace pr
{
	void State::setName(std::string_view name) noexcept
	{
		m_name = static_cast<std::string>(name);
	}

	const std::string& State::getName() const noexcept
	{
		return m_name;
	}

	State::State() noexcept
	    : m_name("null")
	{
	}

	State::State(std::string_view name) noexcept
	    : m_name(static_cast<std::string>(name))
	{
	}

} // namespace pr