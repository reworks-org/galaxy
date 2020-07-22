///
/// State.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "State.hpp"

namespace pr
{
	void State::set_name(std::string_view name) noexcept
	{
		m_name = static_cast<std::string>(name);
	}

	const std::string& State::get_name() const noexcept
	{
		return m_name;
	}

	State::State() noexcept
	    : m_name {"default"}
	{
	}

	State::State(std::string_view name) noexcept
	    : m_name {static_cast<std::string>(name)}
	{
	}

} // namespace pr