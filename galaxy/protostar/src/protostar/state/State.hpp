///
/// State.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_STATE_HPP_
#define PROTOSTAR_STATE_HPP_

#include <string>

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
		/// Copy constructor.
		///
		State(const State&);

		///
		/// Move constructor.
		///
		State(State&&);

		///
		/// Copy assignment operator.
		///
		State& operator=(const State&);

		///
		/// Move assignment operator.
		///
		State& operator=(State&&);

		///
		/// Destructor.
		///
		virtual ~State() = default;

		///
		/// \brief Called when state is pushed.
		///
		virtual void on_push() = 0;

		///
		/// \brief Called when state is popped.
		///
		virtual void on_pop() = 0;

		///
		/// Allows for the state to process events.
		///
		virtual void events() = 0;

		///
		/// \brief Allows for the state to utilize fixed timestep updates.
		///
		/// \param dt Delta-Time from fixed timestep gameloop.
		///
		virtual void update(const double dt) = 0;

		///
		/// Code to be called before rendering. Outside of any glBegin, window.begin(), etc...
		///
		virtual void pre_render() = 0;

		///
		/// Allows for the state to call render code.
		///
		virtual void render() = 0;

		///
		/// Set state name.
		///
		/// \param name Name in std::string format.
		///
		void set_name(std::string_view name);

		///
		/// Get state name.
		///
		/// \return Const std::string.
		///
		const std::string& get_name() const;

	protected:
		///
		/// Default constructor.
		///
		State();

		///
		/// Argument constructor.
		///
		/// \param name Give a name to the state.
		///
		explicit State(std::string_view name);

		///
		/// State name. Useful to have.
		///
		std::string m_name;
	};
} // namespace pr

#endif