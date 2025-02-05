///
/// State.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_STATE_HPP_
#define GALAXY_STATE_STATE_HPP_

namespace galaxy
{
	namespace state
	{
		///
		/// A state to use in a finite state machine.
		///
		class State
		{
		public:
			///
			/// Virtual destructor.
			///
			virtual ~State() = default;

			///
			/// Triggered when state is pushed onto the stack.
			///
			virtual void on_push() = 0;

			///
			/// Triggered when state is popped off the stack.
			///
			virtual void on_pop() = 0;

			///
			/// Handle state events / update state data.
			///
			virtual void update() = 0;

		protected:
			///
			/// Constructor.
			///
			State() = default;
		};
	} // namespace state
} // namespace galaxy

#endif
