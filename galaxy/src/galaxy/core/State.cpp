///
/// State.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "State.hpp"

namespace galaxy
{
	namespace core
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
		    : m_name {"default_state_name"}
		{
		}

		State::State(std::string_view name)
		    : m_name {static_cast<std::string>(name)}
		{
		}
	} // namespace core
} // namespace galaxy