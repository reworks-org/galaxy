///
/// State.hpp
/// starlight
///
/// Created by reworks on 27/09/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_STATE_HPP_
#define STARLIGHT_STATE_HPP_

union ALLEGRO_EVENT;

namespace sl
{
	class State
	{
	public:
		///
		/// Destructor.
		///
		virtual ~State() = default;

		///
		/// Handle any events for this state.
		///
		virtual void event(ALLEGRO_EVENT* event) = 0;

		///
		/// Updates the current state.
		///
		/// \param dt Delta Time from game loop.
		///
		virtual void update(const double dt) = 0;

		///
		/// Render the current state.
		///
		virtual void render() = 0;

	protected:
		///
		/// Default constructor.
		///
		State() = default;
	};
}

#endif