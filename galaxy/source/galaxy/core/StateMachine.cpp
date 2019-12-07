///
/// StateMachine.cpp
/// galaxy
///
/// Created by reworks on 18/05/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "StateMachine.hpp"

namespace galaxy
{
	void StateMachine::load()
	{
		// Check to make sure update is valid to call.
		if (!m_stack.empty() && !m_states.empty())
		{
			m_states[m_stack.top()]->load();
		}
	}

	void StateMachine::unload()
	{
		// Check to make sure update is valid to call.
		if (!m_stack.empty() && !m_states.empty())
		{
			m_states[m_stack.top()]->unload();
		}
	}

	void StateMachine::event(ALLEGRO_EVENT* event)
    {
		// Check to make sure event is valid to call.
		if (!m_stack.empty() && !m_states.empty())
		{
			m_states[m_stack.top()]->event(event);
		}
    }
    
	void StateMachine::update(const double dt)
	{
		// Check to make sure update is valid to call.
		if (!m_stack.empty() && !m_states.empty())
		{
			m_states[m_stack.top()]->update(dt);
		}
	}

	void StateMachine::render()
	{
		// Check to make sure render is valid to call.
		if (!m_stack.empty() && !m_states.empty())
		{
			m_states[m_stack.top()]->render();
		}
	}

	void StateMachine::push(const std::string& state)
	{
		// Ensure that the state being pushed exists.
		if (m_states.find(state) != m_states.end())
		{
			m_stack.push(state);
		}
		else
		{
			LOG_S(WARNING) << "Tried to push a non-existant state: " << state << ". Create it with createState()";
		}
	}

	void StateMachine::pop()
	{
		m_stack.pop();
	}
}