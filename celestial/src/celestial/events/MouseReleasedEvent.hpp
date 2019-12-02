///
/// MouseReleasedEvent.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_MOUSERELEASEDEVENT_HPP_
#define CELESTIAL_MOUSERELEASEDEVENT_HPP_

namespace celestial
{
	///
	/// Contains data relating to a mouse released event.
	///
	struct MouseReleasedEvent final
	{
		///
		/// Constructor.
		///
		/// \param x x-pos of event.
		/// \param y y-pos of event.
		/// \param button Button code pressed.
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