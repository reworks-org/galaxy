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
	StateMachine::StateMachine() noexcept
	{
	}

	StateMachine::~StateMachine() noexcept
	{
		clear();

		m_states.clear();
	}
	
	void StateMachine::events() noexcept
	{
		// Check to make sure event is valid to call.
		if (!m_stack.empty())
		{
			m_stack.top()->events();
		}
		else
		{
			PL_LOG(PL_WARNING, "Tried to access empty statemachine stack.");
		}
	}

	void StateMachine::update(protostar::ProtectedDouble* deltaTime) noexcept
	{
		// Check to make sure update is valid to call.
		if (!m_stack.empty())
		{
			m_stack.top()->update(deltaTime);
		}
		else
		{
			PL_LOG(PL_WARNING, "Tried to access empty statemachine stack.");
		}
	}

	void StateMachine::render() noexcept
	{
		// Check to make sure render is valid to call.
		if (!m_stack.empty())
		{
			m_stack.top()->render();
		}
		else
		{
			PL_LOG(PL_WARNING, "Tried to access empty statemachine stack.");
		}
	}

	void StateMachine::push(const std::string& state) noexcept
	{
		// Ensure that the state being pushed exists.
		if (m_states.find(state) != m_states.end())
		{
			m_stack.push(m_states[state].get());
			m_stack.top()->onPush();
		}
		else
		{
			PL_LOG(PL_WARNING, "Tried to push state that already exists.");
		}
	}

	void StateMachine::pop() noexcept
	{
		// Make sure we dont pop an empty stack...
		if (!m_stack.empty())
		{
			m_stack.top()->onPop();
			m_stack.pop();
		}
		else
		{
			PL_LOG(PL_WARNING, "Tried to access empty statemachine stack.");
		}
	}

	void StateMachine::clear() noexcept
	{
		while (!m_stack.empty())
		{
			m_stack.top()->onPop();
			m_stack.pop();
		}
	}
}