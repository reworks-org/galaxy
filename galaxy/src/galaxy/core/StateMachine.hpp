///
/// StateMachine.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATEMACHINE_HPP_
#define GALAXY_STATEMACHINE_HPP_

#include <memory>
#include <stack>
#include <robin_hood.h>
#include <utility>

#include "galaxy/error/Log.hpp"

#include "galaxy/state/State.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Concept to ensure type is derived from state.
	///
	template<typename Derived>
	concept is_state = std::is_base_of<pr::State, Derived>::value;

	///
	/// A state machine to be used with game states or animations, etc.
	///
	class StateMachine final
	{
	public:
		///
		/// Constructor.
		///
		StateMachine() = default;

		///
		/// Copy constructor.
		///
		StateMachine(const StateMachine&) = delete;

		///
		/// Move constructor.
		///
		StateMachine(StateMachine&&) = delete;

		///
		/// Copy assignment operator.
		///
		StateMachine& operator=(const StateMachine&) = delete;

		///
		/// Move assignment operator.
		///
		StateMachine& operator=(StateMachine&&) = delete;

		///
		/// Destructor.
		///
		~StateMachine();

		///
		/// Pass events onto current state.
		///
		void events();

		///
		/// Updates the current state.
		///
		/// \param dt Delta Time from game loop.
		///
		void update(const double dt);

		///
		/// Code to be called before rendering. Outside of any glBegin, window.begin(), etc...
		///
		void pre_render();

		///
		/// Render the current state.
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
		template<is_state State, typename... Args>
		[[maybe_unused]] State* create(std::string_view name, Args&&... args);

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
		template<typename State>
		[[nodiscard]] State* top();

		///
		/// Clear stack.
		///
		void clear();

		///
		/// Get an array of all state keys.
		///
		/// \return Vector of strings.
		///
		[[nodiscard]] std::vector<std::string> get_state_keys();

	private:
		///
		/// The stack for manipulation.
		///
		std::stack<pr::State*> m_stack;

		///
		/// Holds the states.
		///
		robin_hood::unordered_map<std::string, std::unique_ptr<pr::State>> m_states;
	};

	template<is_state State, typename... Args>
	inline State* StateMachine::create(std::string_view name, Args&&... args)
	{
		const auto str = static_cast<std::string>(name);

		// Construct in place by forwarding arguments to the object.
		if (m_states.contains(str))
		{
			GALAXY_LOG(GALAXY_WARNING, "Attempted to create state that already exists!");
		}
		else
		{
			m_states[str] = std::make_unique<State>(std::forward<Args>(args)...);
			m_states[str]->set_name(name);
		}

		return dynamic_cast<State*>(m_states[str].get());
	}

	template<typename State>
	inline State* StateMachine::top()
	{
		// Ensure stack is not empty.
		if (!m_stack.empty())
		{
			return dynamic_cast<State*>(m_stack.top());
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "No states in stack!");
			return nullptr;
		}
	}
} // namespace galaxy

#endif