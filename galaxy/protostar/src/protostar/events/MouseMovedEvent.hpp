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
		///
		MouseMovedEvent(const double x, const double y) noexcept;

		///
		/// Copy constructor.
		///
		MouseMovedEvent(const MouseMovedEvent&) noexcept = default;

		///
		/// Move constructor.
		///
		MouseMovedEvent(MouseMovedEvent&&) noexcept = default;

		///
		/// Copy assignment operator.
		///
		MouseMovedEvent& operator=(const MouseMovedEvent&) noexcept = default;

		///
		/// Move assignment operator.
		///
		MouseMovedEvent& operator=(MouseMovedEvent&&) noexcept = default;

		///
		/// Default destructor.
		///
		~MouseMovedEvent() noexcept = default;

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