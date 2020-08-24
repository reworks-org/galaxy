///
/// MouseWheelEvent.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_MOUSEWHEELEVENT_HPP_
#define PROTOSTAR_MOUSEWHEELEVENT_HPP_

///
/// Core namespace.
///
namespace pr
{
	///
	/// Contains mouse wheel movement data.
	///
	struct MouseWheelEvent final
	{
		///
		/// Default constructor.
		///
		MouseWheelEvent() noexcept;

		///
		/// Constructor.
		///
		/// \param x x offset.
		/// \param y y offset.
		///
		MouseWheelEvent(const double x, const double y) noexcept;

		///
		/// Copy constructor.
		///
		MouseWheelEvent(const MouseWheelEvent&) noexcept = default;

		///
		/// Move constructor.
		///
		MouseWheelEvent(MouseWheelEvent&&) noexcept = default;

		///
		/// Copy assignment operator.
		///
		MouseWheelEvent& operator=(const MouseWheelEvent&) noexcept = default;

		///
		/// Move assignment operator.
		///
		MouseWheelEvent& operator=(MouseWheelEvent&&) noexcept = default;

		///
		/// Default destructor.
		///
		~MouseWheelEvent() noexcept = default;

		///
		/// X offset of scroll.
		///
		double m_x_offset;

		///
		/// Y offset of scroll.
		///
		double m_y_offset;
	};
} // namespace pr

#endif