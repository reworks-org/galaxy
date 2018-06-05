///
/// StateMachine.hpp
/// starlight
///
/// Created by reworks on 18/05/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_STATEMACHINE_HPP_
#define STARLIGHT_STATEMACHINE_HPP_

#include <stack>
#include <memory>
#include <utility>
#include <string_view>
#include <unordered_map>

#include "sl/types/State.hpp"

namespace sl
{
	class StateMachine final
	{
	public:
		///
		/// Constructor.
		///
		StateMachine() = default;

		///
		/// Destructor.
		///
		~StateMachine() = default;
	    
        ///
		/// Poll for events in the current state.
		///
		void event(ALLEGRO_EVENT* event);
        
		///
		/// Updates the current state.
		///
		/// \param dt Delta Time from game loop.
		///
		void update(const double dt);

		///
		/// Render the current state.
		///
		void render();

		///
		/// \brief Add a new state to the machine.
		///
		/// Template Typename State is the type of the state to add and 
		/// the args are the arguments to construct that state.
		///
		/// \param id Name of the state to identify it by.
		/// \param args Arguments to construct the state.
		///
		template<typename State, typename ... Args>
		void addState(std::string_view id, Args&&... args);

		///
		/// Push a new state to the top of the stack.
		///
		/// \param state State ID to push.
		///
		void push(std::string_view state);

		///
		/// Pop the top state.
		///
		void pop();

		///
		/// \brief The current top state.
		///
		/// \return Returns StateType* pointer or just a State* by default.
		///
		template<typename State = State>
		State* top();

	private:
		std::stack<std::string_view> m_stack;
		std::unordered_map<std::string_view, std::unique_ptr<State>> m_states;
	};

	template<typename State, typename ... Args>
	void StateMachine::addState(std::string_view id, Args&&... args)
	{
		m_states.emplace(id, std::make_unique<State>(std::forward<Args>(args)...));
	}

	template<typename State>
	State* StateMachine::top()
	{
		#ifdef NDEBUG
			return m_states[m_stack.top()].get();
		#else
			if (!m_stack.empty() && !m_states.empty())
			{
				return m_states[m_stack.top()].get();
			}
			else
			{
				return nullptr;
			}
		#endif
	}
}

#endif