///
/// State.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "State.hpp"

namespace galaxy
{
	void State::set_name(std::string_view name)
	{
		m_name = static_cast<std::string>(name);
	}

	const std::string& State::get_name() const
	{
		return m_name;
	}

	LayerStorage* State::get_layers()
	{
		return &m_layers;
	}

	State::State()
	    : m_name {"default"}
	{
	}

	State::State(std::string_view name)
	    : m_name {static_cast<std::string>(name)}
	{
	}

	State::State(const State& s)
	{
		this->m_name = s.m_name;
	}

	State::State(State&& s)
	{
		this->m_name = s.m_name;
		s.m_name     = "";
	}

	State& State::operator=(const State& s)
	{
		this->m_name = s.m_name;
		return *this;
	}

	State& State::operator=(State&& s)
	{
		if (this != &s)
		{
			this->m_name = s.m_name;
			s.m_name     = "";
		}

		return *this;
	}

} // namespace galaxy