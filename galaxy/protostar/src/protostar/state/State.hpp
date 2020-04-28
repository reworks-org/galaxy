///
/// State.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_STATE_HPP_
#define PROTOSTAR_STATE_HPP_

#include <any>
#include <string>

///
/// Core namespace.
///
namespace protostar
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
		virtual void onPush() noexcept = 0;

		///
		/// \brief Called when state is popped.
		///
		virtual void onPop() noexcept = 0;

		///
		/// \brief Allows for the state to process events.
		///
		/// \param event Is a std::any to allow any type of event object to be passed.
		///
		virtual void event(const std::any& event) noexcept = 0;

		///
		/// \brief Allows for the state to utilize fixed timestep updates.
		///
		/// \param dt Delta-Time from fixed timestep gameloop.
		///
		virtual void update(const double dt) noexcept = 0;

		///
		/// \brief Allows for the state to call render code.
		///
		virtual void render() noexcept = 0;

		///
		/// Set state name.
		///
		/// \param name Name in std::string format.
		///
		virtual void setName(const std::string& name) noexcept final;

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
		explicit State(const std::string& name) noexcept;

	protected:
		///
		/// State name. Useful to have.
		///
		std::string m_name;
	};
}

#endif