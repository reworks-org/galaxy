///
/// MousePressedEvent.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_MOUSEPRESSEDEVENT_HPP_
#define PROTOSTAR_MOUSEPRESSEDEVENT_HPP_

///
/// Core namespace.
///
namespace pr
{
	///
	/// Contains data relating to a mouse pressed event.
	///
	struct MousePressedEvent final
	{
		///
		/// Default constructor.
		///
		MousePressedEvent();

		///
		/// Constructor.
		///
		/// \param x x-pos of event.
		/// \param y y-pos of event.
		/// \param button Button code pressed.
		///
		MousePressedEvent(const float x, const float y, const int button);

		///
		/// Copy constructor.
		///

		MousePressedEvent(const MousePressedEvent&) = default;
		///
		/// Move constructor.
		///
		MousePressedEvent(MousePressedEvent&&) = default;

		///
		/// Copy assignment operator.
		///
		MousePressedEvent& operator=(const MousePressedEvent&) = default;

		///
		/// Move assignment operator.
		///
		MousePressedEvent& operator=(MousePressedEvent&&) = default;

		///
		/// Default destructor.
		///
		~MousePressedEvent() = default;

		///
		/// Mouse x position.
		///
		float m_x;

		///
		/// Mouse y position.
		///
		float m_y;

		///
		/// Mouse button pressed.
		///
		int m_button;
	};
} // namespace pr

#endif