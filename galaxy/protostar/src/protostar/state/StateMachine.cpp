///
/// StateMachine.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "StateMachine.hpp"

///
/// Core namespace.
///
namespace protostar
{
	StateMachine::~StateMachine()
	{
		while (!m_stack.empty())
		{
			m_stack.pop();
		}

		m_states.clear();
	}
	
	void StateMachine::event(const std::any& event)
	{
		// Check to make sure event is valid to call.
		if (!m_stack.empty())
		{
			m_stack.top()->event(event);
		}
	}

	void StateMachine::update(const double dt)
	{
		// Check to make sure update is valid to call.
		if (!m_stack.empty())
		{
			m_stack.top()->update(dt);
		}
	}

	void StateMachine::render() noexcept
	{
		// Check to make sure render is valid to call.
		if (!m_stack.empty())
		{
			m_stack.top()->render();
		}
	}

	void StateMachine::push(const std::string& state)
	{
		// Ensure that the state being pushed exists.
		if (m_states.find(state) != m_states.end())
		{
			m_stack.push(m_states[state].get());
			m_stack.top()->onPush();
		}
	}

	void StateMachine::pop()
	{
		// Make sure we dont pop an empty stack...
		if (!m_stack.empty())
		{
			m_stack.top()->onPop();
			m_stack.pop();
		}
	}
}