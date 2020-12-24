///
/// MouseReleased.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSERELEASED_HPP_
#define GALAXY_EVENTS_MOUSERELEASED_HPP_

#include "galaxy/input/MouseButtons.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Contains data relating to a mouse released event.
		///
		struct MouseReleased final
		{
			///
			/// Default constructor.
			///
			MouseReleased();

			///
			/// Constructor.
			///
			/// \param x x-pos of event.
			/// \param y y-pos of event.
			/// \param button Button code pressed.
			///
			MouseReleased(const float x, const float y, const input::MouseButton button);

			///
			/// Default destructor.
			///
			~MouseReleased() = default;

			///
			/// Mouse x position.
			///
			float m_x;

			///
			/// Mouse y position.
			///
			float m_y;

			///
			/// Mouse button released.
			///
			input::MouseButton m_button;
		};
	} // namespace events
} // namespace galaxy

#endif