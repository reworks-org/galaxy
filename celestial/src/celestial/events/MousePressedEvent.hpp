///
/// MousePressedEvent.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_MOUSEPRESSEDEVENT_HPP_
#define CELESTIAL_MOUSEPRESSEDEVENT_HPP_

namespace celestial
{
	///
	/// Contains data relating to a mouse pressed event.
	///
	struct MousePressedEvent final
	{
		///
		/// Constructor.
		///
		/// \param x x-pos of event.
		/// \param y y-pos of event.
		/// \param button Button code pressed.
		///
		inline MousePressedEvent(const int x, const int y, const unsigned int button) noexcept
			:m_x(x), m_y(y), m_button(button)
		{
		}

		///
		/// Mouse x position.
		///
		int m_x = 0;

		///
		/// Mouse y position.
		///
		int m_y = 0;
		
		///
		/// Mouse button pressed.
		///
		unsigned int m_button = 0;
	};
}

#endif