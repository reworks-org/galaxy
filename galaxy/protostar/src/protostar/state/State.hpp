///
/// State.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_STATE_HPP_
#define PROTOSTAR_STATE_HPP_

#include <string>

#include "protostar/async/ProtectedArithmetic.hpp"

///
/// Core namespace.
///
namespace pr
{
	///
	/// Represents one of many possible object states.
	///
	class State
	{
	public:
		///
		/// Default virtual destructor.
		///
		virtual ~State() noexcept = default;

		///
		/// \brief Called when state is pushed.
		///
		virtual void onPush() = 0;

		///
		/// \brief Called when state is popped.
		///
		virtual void onPop() = 0;

		///
		/// Allows for the state to process events.
		///
		virtual void events() = 0;

		///
		/// \brief Allows for the state to utilize fixed timestep updates.
		///
		/// \param dt Delta-Time from fixed timestep gameloop.
		///
		virtual void update(pr::ProtectedDouble* dt) = 0;

		///
		/// \brief Allows for the state to call render code.
		///
		virtual void render() = 0;

		///
		/// Set state name.
		///
		/// \param name Name in std::string format.
		///
		virtual void setName(std::string_view name) noexcept final;

		///
		/// Get state name.
		///
		/// \return Const std::string.
		///
		virtual const std::string& getName() const noexcept final;

	protected:
		///
		/// Default constructor.
		///
		State() noexcept;

		///
		/// Argument constructor.
		///
		/// \param name Give a name to the state.
		///
		explicit State(std::string_view name) noexcept;

		///
		/// State name. Useful to have.
		///
		std::string m_name;
	};
}

#endif