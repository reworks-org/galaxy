///
/// MouseMovedEvent.hpp
/// galaxy
///
/// Created by reworks on 14/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MOUSEMOVEDEVENT_HPP_
#define GALAXY_MOUSEMOVEDEVENT_HPP_

namespace galaxy
{
	///
	/// Contains data relating to a mouse moved event.
	///
	struct MouseMovedEvent final
	{
		///
		/// Constructor.
		///
		inline MouseMovedEvent(const int x, const int y, const int dx, const int dy, const float pressure) noexcept
			:m_x(x), m_y(y), m_dx(dx), m_dy(dy), m_pressure(pressure)
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
		/// Change in x coord value since previous event.
		///
		int m_dx = 0;

		///
		/// Change in y coord value since previous event.
		///
		int m_dy = 0;

		///
		/// Any pressure from any button click the mouse might be applying.
		///
		float m_pressure = 0.0f;
	};
}

#endif