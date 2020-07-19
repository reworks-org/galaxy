///
/// MouseMovedEvent.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_MOUSEMOVEDEVENT_HPP_
#define PROTOSTAR_MOUSEMOVEDEVENT_HPP_

///
/// Core namespace.
///
namespace pr
{
	///
	/// Contains data relating to a mouse moved event.
	///
	struct MouseMovedEvent final
	{
		///
		/// Default constructor.
		///
		MouseMovedEvent() noexcept;

		///
		/// Constructor.
		///
		/// \param x x-pos of event.
		/// \param y y-pos of event.
		/// \param dx Change in y coord value since previous event.
		/// \param dy Change in y coord value since previous event.
		/// \param pressure Any pressure from any button click the mouse might be applying.
		///
		MouseMovedEvent(const int x, const int y, const int dx, const int dy, const float pressure) noexcept;

		///
		/// Default destructor.
		///
		~MouseMovedEvent() noexcept = default;

		///
		/// Mouse x position.
		///
		int m_x;

		///
		/// Mouse y position.
		///
		int m_y;

		///
		/// Change in x coord value since previous event.
		///
		int m_dx;

		///
		/// Change in y coord value since previous event.
		///
		int m_dy;

		///
		/// Any pressure from any button click the mouse might be applying.
		///
		float m_pressure;
	};
} // namespace pr

#endif