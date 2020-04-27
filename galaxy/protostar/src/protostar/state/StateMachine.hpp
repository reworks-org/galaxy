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
#include <stdexcept>
#include <unordered_map>

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
		StateMachine() = default;

		///
		/// Destructor.
		///
		~StateMachine();

		///
		/// \brief Pass events onto current state.
		///
		/// Allows for std::exceptions.
		///
		/// \param event Is a std::any to allow any type of event object to be passed.
		///
		void event(const std::any& event);
        
		///
		/// Updates the current state.
		///
		/// \param dt Delta Time from game loop.
		///
		void update(const double dt);

		///
		/// \brief Render the current state.
		///
		/// Cannot throw std::exceptions.
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
		State* create(const std::string& name, Args&&... args);

		///
		/// Push a new state to the top of the stack.
		///
		/// \param state Name of state to push.
		///
		void push(const std::string& state);

		///
		/// Pop top state.
		///
		void pop();

		///
		/// Get the state on top of the stack.
		///
		/// \return Returns pointer to topmost state.
		///
		template<typename State>
		State* top();

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
	inline State* StateMachine::create(const std::string& name, Args&& ...args)
	{
		// Construct in place by forwarding arguments to the object.
		m_states.emplace(name, std::make_unique<State>(std::forward<Args>(args)...));

		return dynamic_cast<State*>(m_states[name].get());
	}

	template<typename State>
	inline State* StateMachine::top()
	{
		State* output = nullptr;

		// Ensure stack is not empty.
		if (!m_stack.empty())
		{
			output = dynamic_cast<State*>(m_stack.top());
		}
		else
		{
			throw std::runtime_error("No states in stack!");
		}

		return output;
	}
}

#endif