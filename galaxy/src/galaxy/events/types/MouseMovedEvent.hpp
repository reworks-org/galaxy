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
		MouseMovedEvent();

		///
		/// Constructor.
		///
		/// \param x x-pos of event.
		/// \param y y-pos of event.
		///
		MouseMovedEvent(const double x, const double y);

		///
		/// Copy constructor.
		///
		MouseMovedEvent(const MouseMovedEvent&) = default;

		///
		/// Move constructor.
		///
		MouseMovedEvent(MouseMovedEvent&&) = default;

		///
		/// Copy assignment operator.
		///
		MouseMovedEvent& operator=(const MouseMovedEvent&) = default;

		///
		/// Move assignment operator.
		///
		MouseMovedEvent& operator=(MouseMovedEvent&&) = default;

		///
		/// Default destructor.
		///
		~MouseMovedEvent() = default;

		///
		/// Mouse x position.
		///
		double m_x;

		///
		/// Mouse y position.
		///
		double m_y;
	};
} // namespace pr

#endif