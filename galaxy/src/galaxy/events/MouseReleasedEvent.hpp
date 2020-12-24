///
/// MouseReleasedEvent.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_MOUSERELEASEDEVENT_HPP_
#define PROTOSTAR_MOUSERELEASEDEVENT_HPP_

#include "protostar/system/MouseButtons.hpp"

///
/// Core namespace.
///
namespace pr
{
	///
	/// Contains data relating to a mouse released event.
	///
	struct MouseReleasedEvent final
	{
		///
		/// Default constructor.
		///
		MouseReleasedEvent();

		///
		/// Constructor.
		///
		/// \param x x-pos of event.
		/// \param y y-pos of event.
		/// \param button Button code pressed.
		///
		MouseReleasedEvent(const float x, const float y, const pr::MouseButton button);

		///
		/// Copy constructor.
		///
		MouseReleasedEvent(const MouseReleasedEvent&) = default;

		///
		/// Move constructor.
		///
		MouseReleasedEvent(MouseReleasedEvent&&) = default;

		///
		/// Copy assignment operator.
		///
		MouseReleasedEvent& operator=(const MouseReleasedEvent&) = default;

		///
		/// Move assignment operator.
		///
		MouseReleasedEvent& operator=(MouseReleasedEvent&&) = default;

		///
		/// Default destructor.
		///
		~MouseReleasedEvent() = default;

		///
		/// Mouse x position.
		///
		float m_x;

		///
		/// Mouse y position.
		///
		float m_y;

		///
		/// Mouse button released.
		///
		pr::MouseButton m_button;
	};
} // namespace pr

#endif