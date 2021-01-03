///
/// StateMachine.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "StateMachine.hpp"

namespace galaxy
{
	namespace core
	{
		StateMachine::~StateMachine()
		{
			clear();
			m_states.clear();
		}

		void StateMachine::events()
		{
			// Check to make sure event is valid to call.
			if (!m_stack.empty())
			{
				m_stack.top()->events();
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to access empty StateMachine stack.");
			}
		}

		void StateMachine::update(const double dt)
		{
			// Check to make sure update is valid to call.
			if (!m_stack.empty())
			{
				m_stack.top()->update(dt);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to access empty StateMachine stack.");
			}
		}

		void StateMachine::pre_render()
		{
			// Check to make sure pre_render is valid to call.
			if (!m_stack.empty())
			{
				m_stack.top()->pre_render();
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to access empty StateMachine stack.");
			}
		}

		void StateMachine::render()
		{
			// Check to make sure render is valid to call.
			if (!m_stack.empty())
			{
				m_stack.top()->render();
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to access empty StateMachine stack.");
			}
		}

		void StateMachine::push(std::string_view state)
		{
			const auto str = static_cast<std::string>(state);

			// Ensure that the state being pushed exists.
			if (m_states.contains(str))
			{
				if (!m_stack.empty())
				{
					if (m_stack.top()->get_name() != str)
					{
						m_stack.push(m_states[str].get());
						m_stack.top()->on_push();
					}
					else
					{
						GALAXY_LOG(GALAXY_WARNING, "Tried to push duplicate state.");
					}
				}
				else
				{
					m_stack.push(m_states[str].get());
					m_stack.top()->on_push();
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to push state that already exists.");
			}
		}

		void StateMachine::pop()
		{
			// Make sure we dont pop an empty stack...
			// Or the only current state.
			if (m_stack.size() > 1)
			{
				m_stack.top()->on_pop();
				m_stack.pop();
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to pop base state.");
			}
		}

		void StateMachine::clear()
		{
			while (!m_stack.empty())
			{
				m_stack.top()->on_pop();
				m_stack.pop();
			}
		}

		std::vector<std::string> StateMachine::get_state_keys()
		{
			std::vector<std::string> keys;
			keys.reserve(m_states.size());

			std::transform(m_states.begin(), m_states.end(), std::back_inserter(keys), [](const auto& pair) {
				return pair.first;
			});

			return keys;
		}
	} // namespace core
} // namespace galaxy