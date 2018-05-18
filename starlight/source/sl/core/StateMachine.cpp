///
/// StateMachine.cpp
/// starlight
///
/// Created by reworks on 18/05/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

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
			if (!m_stack.empty() && !m_states.empty())
			{
				m_states[m_stack.top()]->event(event);
			}
		#endif
    }
    
	void StateMachine::update(const double dt)
	{
		#ifdef NDEBUG
			m_states[m_stack.top()]->update(dt);
		#else
			if (!m_stack.empty() && !m_states.empty())
			{
				m_states[m_stack.top()]->update(dt);
			}
		#endif
	}

	void StateMachine::render()
	{
		#ifdef NDEBUG
			m_states[m_stack.top()]->render();
		#else
			if (!m_stack.empty() && !m_states.empty())
			{
				m_states[m_stack.top()]->render();
			}
		#endif
	}

	void StateMachine::push(std::string_view state)
	{
		#ifdef NDEBUG
			m_stack.push(state);
		#else
			if (m_states.find(state) != m_states.end())
			{
				m_stack.push(state);
			}
			else
			{
				LOG_S(WARNING) << "Tried to push a non-existant state: " << state;
			}
		#endif
	}

	void StateMachine::pop()
	{
		m_stack.pop();
	}
}