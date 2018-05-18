///
/// StateMachine.cpp
/// starlight
///
/// Created by reworks on 18/05/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/types/State.hpp"

#ifndef NDEBUG
	#include "sl/libs/loguru/loguru.hpp"
#endif

#include "StateMachine.hpp"

namespace sl
{
    void StateMachine::event(ALLEGRO_EVENT* event)
    {
		#ifdef NDEBUG
			m_states[m_stack.top()]->event(event);
		#else
			if (!m_stack.empty())
			{
				m_states[m_stack.top()]->event(event);
			}
		#endif
    }
    
	void StateMachine::update(const double dt)
	{
		#ifdef NDEBUG
			m_stack.top()->update(dt);
		#else
			if (!m_stack.empty())
			{
				m_stack.top()->update(dt);
			}
		#endif
	}

	void StateMachine::render()
	{
		#ifdef NDEBUG
			m_stack.top()->render();
		#else
			if (!m_stack.empty())
			{
				m_stack.top()->render();
			}
		#endif
	}

	void StateMachine::push(entt::HashedString state)
	{
		#ifdef NDEBUG
			m_stack.push(m_states[state].get());
		#else
			if (m_states.find(state) != m_states.end())
			{
				m_stack.push(m_states[state].get());
			}
			else
			{
				LOG_S(WARNING) << "Tried to push a non-existant state: " << *state;
			}
		#endif
	}

	void StateMachine::pop()
	{
		m_stack.pop();
	}
}