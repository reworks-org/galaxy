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
namespace protostar
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
		MouseWheelEvent(const float x, const float y) noexcept;

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
}

#endif