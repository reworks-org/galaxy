///
/// State.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_STATE_HPP_
#define GALAXY_STATE_STATE_HPP_

#include <string>

namespace galaxy
{
	///
	/// A state to use in a finite state machine.
	///
	class State
	{
	public:
		///
		/// Move constructor.
		///
		State(State&&) = default;

		///
		/// Move assignment operator.
		///
		State& operator=(State&&) = default;

		///
		/// Copy constructor.
		///
		State(const State&) = default;

		///
		/// Copy assignment operator.
		///
		State& operator=(const State&) = default;

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
		/// Get state name.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		const std::string& name() const noexcept;

	protected:
		///
		/// Name constructor.
		///
		/// \param name Name of the scene for debug / editor purposes.
		///
		State(const std::string& name) noexcept;

	protected:
		///
		/// Name for debug purposes.
		///
		std::string m_name;

	private:
		///
		/// Constructor.
		///
		State() = delete;
	};
} // namespace galaxy

#endif
