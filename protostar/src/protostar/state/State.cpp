///
/// State.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "State.hpp"

namespace protostar
{
	void State::setName(const std::string& name) noexcept
	{
		m_name = name;
	}

	const std::string& State::getName() noexcept
	{
		return m_name;
	}

	State::State() noexcept
		:m_name("Null")
	{
	}

	State::State(const std::string& name) noexcept
		:m_name(name)
	{
	}

}