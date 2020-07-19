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
		/// Default destructor.
		///
		~MouseWheelEvent() noexcept = default;

		///
		/// X offset of scroll.
		///
		double m_xOffset;

		///
		/// Y offset of scroll.
		///
		double m_yOffset;
	};
} // namespace pr

#endif