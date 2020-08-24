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
		MousePressedEvent() noexcept;

		///
		/// Constructor.
		///
		/// \param x x-pos of event.
		/// \param y y-pos of event.
		/// \param button Button code pressed.
		///
		MousePressedEvent(const int x, const int y, const unsigned int button) noexcept;

		///
		/// Copy constructor.
		///

		MousePressedEvent(const MousePressedEvent&) noexcept = default;
		///
		/// Move constructor.
		///
		MousePressedEvent(MousePressedEvent&&) noexcept = default;

		///
		/// Copy assignment operator.
		///
		MousePressedEvent& operator=(const MousePressedEvent&) noexcept = default;

		///
		/// Move assignment operator.
		///
		MousePressedEvent& operator=(MousePressedEvent&&) noexcept = default;

		///
		/// Default destructor.
		///
		~MousePressedEvent() noexcept = default;

		///
		/// Mouse x position.
		///
		int m_x;

		///
		/// Mouse y position.
		///
		int m_y;

		///
		/// Mouse button pressed.
		///
		unsigned int m_button;
	};
} // namespace pr

#endif