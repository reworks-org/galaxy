///
/// MousePressed.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSEPRESSED_HPP_
#define GALAXY_EVENTS_MOUSEPRESSED_HPP_

#include "galaxy/input/MouseButtons.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Contains data relating to a mouse pressed event.
		///
		struct MousePressed final
		{
			///
			/// Default constructor.
			///
			MousePressed() noexcept;

			///
			/// Constructor.
			///
			/// \param x x-pos of event.
			/// \param y y-pos of event.
			/// \param button Button code pressed.
			///
			MousePressed(const float x, const float y, const input::MouseButtons button) noexcept;

			///
			/// Default destructor.
			///
			~MousePressed() noexcept = default;

			///
			/// Mouse x position.
			///
			float m_x;

			///
			/// Mouse y position.
			///
			float m_y;

			///
			/// Mouse button pressed.
			///
			input::MouseButtons m_button;
		};
	} // namespace events
} // namespace galaxy

#endif