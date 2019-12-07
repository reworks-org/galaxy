///
/// State.hpp
/// galaxy
///
/// Created by reworks on 27/09/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_HPP_
#define GALAXY_STATE_HPP_

#include "galaxy/math/Rect.hpp"

union ALLEGRO_EVENT;

galaxy
{
	///
	/// Represents a "state" of the application i.e. gamestate, battlestate, menustate, etc...
	///
	class State
	{
	public:
		///
		/// Destructor.
		///
		virtual ~State() noexcept = default;

		///
		/// Load the current state resources.
		///
		virtual void load() = 0;

		///
		/// Unloads the current state resources.
		///
		virtual void unload() = 0;

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

		///
		/// Default constructor.
		///
		/// \param bounds Rendering bounds for this state.
		///
		inline State(const galaxyRect<float, int>& bounds) : m_bounds(bounds) {}

	public:
		///
		/// The dimensions for the state, used with a quadtree to decide what to render to screen.
		/// This could be the screen size or the map size for example.
		///
		Rect<float, int> m_bounds;
	};
}

#endif