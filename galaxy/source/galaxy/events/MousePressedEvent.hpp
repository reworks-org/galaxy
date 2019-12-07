///
/// MousePressedEvent.hpp
/// galaxy
///
/// Created by reworks on 14/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MOUSEPRESSEDEVENT_HPP_
#define GALAXY_MOUSEPRESSEDEVENT_HPP_

namespace galaxy
{
	///
	/// Contains data relating to a mouse pressed event.
	///
	struct MousePressedEvent final
	{
		///
		/// Constructor.
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