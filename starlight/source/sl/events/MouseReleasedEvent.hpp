///
/// MouseReleasedEvent.hpp
/// starlight
///
/// Created by reworks on 14/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_MOUSERELEASEDEVENT_HPP_
#define STARLIGHT_MOUSERELEASEDEVENT_HPP_

namespace sl
{
	///
	/// Contains data relating to a mouse released event.
	///
	struct MouseReleasedEvent final
	{
		///
		/// Constructor.
		///
		inline MouseReleasedEvent(const int x, const int y, const unsigned int button) noexcept
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
		/// Mouse button released.
		///
		unsigned int m_button = 0;
	};
}

#endif