///
/// StateMachine.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"

#include "StateMachine.hpp"

namespace galaxy
{
	namespace state
	{
		StateMachine::StateMachine()
		{
		}

		StateMachine::~StateMachine()
		{
			while (!m_stack.empty())
			{
				m_stack.pop();
			}

			m_states.clear();
		}

		void StateMachine::push(const std::string& key)
		{
			if (m_states.contains(key))
			{
				auto ptr = m_states[key].get();
				ptr->on_push();

				m_stack.push(ptr);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to push a state that doesnt exist.");
			}
		}

		void StateMachine::pop()
		{
			if (m_stack.top() != nullptr)
			{
				m_stack.top()->on_pop();
			}

			m_stack.pop();
		}

		void StateMachine::update()
		{
			if (m_stack.top() != nullptr)
			{
				m_stack.top()->update();
			}
		}
	} // namespace state
} // namespace galaxy
