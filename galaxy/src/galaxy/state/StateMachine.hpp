///
/// StateMachine.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_STATEMACHINE_HPP_
#define GALAXY_STATE_STATEMACHINE_HPP_

#include <memory>
#include <stack>
#include <robin_hood.h>

#include "galaxy/error/Log.hpp"
#include "galaxy/state/State.hpp"

namespace galaxy
{
	namespace state
	{
		///
		/// \brief A finite state machine.
		///
		/// https://gameprogrammingpatterns.com/state.html
		///
		class StateMachine final
		{
			///
			/// Ensure stack is using a vector over slower data structures.
			///
			using Stack = std::stack<State*, std::vector<State*>>;

		public:
			///
			/// Constructor.
			///
			StateMachine() noexcept;

			///
			/// Destructor.
			///
			~StateMachine() noexcept;

			///
			/// Add a new state.
			///
			/// \tparam Type Child state type.
			/// \tparam Args Variable argument types.
			///
			/// \param key Key to assign to this state.
			/// \param args Variable arguments for state constructor.
			///
			template<std::derived_from<State> Type, typename... Args>
			void add(const std::string& key, Args&&... args) noexcept;

			///
			/// Push a new state onto the stack.
			///
			/// \param key Key of state to push.
			///
			void push(const std::string& key) noexcept;

			///
			/// Pop the topmost state off the top of the stack.
			///
			void pop() noexcept;

			///
			/// Update topmost state.
			///
			void update();

		private:
			///
			/// State stack.
			///
			Stack m_stack;

			///
			/// State cache.
			///
			robin_hood::unordered_node_map<std::string, std::unique_ptr<State>> m_states;
		};

		template<std::derived_from<State> Type, typename... Args>
		inline void StateMachine::add(const std::string& key, Args&&... args) noexcept
		{
			m_states[key] = std::make_unique<Type>(std::forward<Args>(args)...);
		}
	} // namespace state
} // namespace galaxy

#endif