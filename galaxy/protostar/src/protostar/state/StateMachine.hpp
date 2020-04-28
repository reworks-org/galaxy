///
/// StateMachine.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_STATEMACHINE_HPP_
#define PROTOSTAR_STATEMACHINE_HPP_

#include <stack>
#include <memory>
#include <utility>
#include <unordered_map>

#include <pulsar/Log.hpp>

#include "protostar/state/State.hpp"

///
/// Core namespace.
///
namespace protostar
{
	///
	/// A state machine to be used with game states or animations, etc.
	///
	class StateMachine final
	{
	public: 
		///
		/// Default constructor.
		///
		StateMachine() noexcept = default;

		///
		/// Destructor.
		///
		~StateMachine() noexcept;

		///
		/// \brief Pass events onto current state.
		///
		/// \param event Is a std::any to allow any type of event object to be passed.
		///
		void event(const std::any& event) noexcept;
        
		///
		/// Updates the current state.
		///
		/// \param dt Delta Time from game loop.
		///
		void update(const double dt) noexcept;

		///
		/// \brief Render the current state.
		///
		void render() noexcept;

		///
		/// \brief Create a new state to store.
		///
		/// Template Typename State is the type of the state to add and 
		/// the args are the arguments to construct that state.
		///
		/// \param name Name of the state to identify it by.
		/// \param args Arguments to construct the state.
		///
		/// \return Returns pointer to newly created state.
		///
		template<typename State, typename ... Args>
		State* create(const std::string& name, Args&&... args) noexcept;

		///
		/// Push a new state to the top of the stack.
		///
		/// \param state Name of state to push.
		///
		void push(const std::string& state) noexcept;

		///
		/// Pop top state.
		///
		void pop() noexcept;

		///
		/// Get the state on top of the stack.
		///
		/// \return Returns pointer to topmost state.
		///
		template<typename State>
		State* top() noexcept;

	private:
		///
		/// The stack for manipulation.
		///
		std::stack<protostar::State*> m_stack;

		///
		/// Holds the states.
		///
		std::unordered_map<std::string, std::unique_ptr<protostar::State>> m_states;
	};

	template<typename State, typename ...Args>
	inline State* StateMachine::create(const std::string& name, Args&& ...args) noexcept
	{
		// Construct in place by forwarding arguments to the object.
		if (m_states.find(name) != m_states.end())
		{
			PL_LOG(PL_ERROR, "Attempted to create state that already exists!");
		}
		else
		{
			m_states.emplace(name, std::make_unique<State>(std::forward<Args>(args)...));
		}
		
		return dynamic_cast<State*>(m_states[name].get());
	}

	template<typename State>
	inline State* StateMachine::top() noexcept
	{
		// Ensure stack is not empty.
		if (!m_stack.empty())
		{
			return dynamic_cast<State*>(m_stack.top());
		}
		else
		{
			PL_LOG(PL_WARNING, "No states in stack!");
			return nullptr;
		}
	}
}

#endif