///
/// StateMachine.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_STATEMACHINE_HPP_
#define PROTOSTAR_STATEMACHINE_HPP_

#include <memory>
#include <stack>
#include <unordered_map>
#include <utility>

#include <pulsar/Log.hpp>

#include "protostar/state/State.hpp"

///
/// Core namespace.
///
namespace pr
{
	///
	/// Concept to ensure type is derived from state.
	///
	template<typename derived>
	concept is_state = std::is_base_of<pr::State, derived>::value;

	///
	/// A state machine to be used with game states or animations, etc.
	///
	class StateMachine final
	{
	public:
		///
		/// Default constructor.
		///
		StateMachine() noexcept;

		///
		/// Destructor.
		///
		~StateMachine() noexcept;

		///
		/// Pass events onto current state.
		///
		void events();

		///
		/// Updates the current state.
		///
		/// \param dt Delta Time from game loop.
		///
		void update(pr::ProtectedDouble* dt);

		///
		/// \brief Render the current state.
		///
		void render();

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
		template<is_state state, typename... _args>
		[[maybe_unused]] state* create(std::string_view name, _args&&... args);

		///
		/// Push a new state to the top of the stack.
		///
		/// \param state Name of state to push.
		///
		void push(std::string_view state);

		///
		/// Pop top state.
		///
		void pop();

		///
		/// Get the state on top of the stack.
		///
		/// \return Returns pointer to topmost state.
		///
		template<typename state>
		[[nodiscard]] state* top() noexcept;

		///
		/// Clear stack.
		///
		void clear();

	private:
		///
		/// The stack for manipulation.
		///
		std::stack<pr::State*> m_stack;

		///
		/// Holds the states.
		///
		std::unordered_map<std::string, std::unique_ptr<pr::State>> m_states;
	};

	template<is_state state, typename... _args>
	inline state* StateMachine::create(std::string_view name, _args&&... args)
	{
		const auto str = static_cast<std::string>(name);

		// Construct in place by forwarding arguments to the object.
		if (m_states.contains(str))
		{
			PL_LOG(PL_WARNING, "Attempted to create state that already exists!");
		}
		else
		{
			m_states[str] = std::make_unique<state>(std::forward<_args>(args)...);
		}

		return dynamic_cast<state*>(m_states[str].get());
	}

	template<typename state>
	inline state* StateMachine::top() noexcept
	{
		// Ensure stack is not empty.
		if (!m_stack.empty())
		{
			return dynamic_cast<State*>(m_stack.top());
		}
		else
		{
			PL_LOG(PL_ERROR, "No states in stack!");
			return nullptr;
		}
	}
} // namespace pr

#endif