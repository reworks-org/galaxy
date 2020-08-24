///
/// MouseReleasedEvent.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_MOUSERELEASEDEVENT_HPP_
#define PROTOSTAR_MOUSERELEASEDEVENT_HPP_

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
		MouseReleasedEvent() noexcept;

		///
		/// Constructor.
		///
		/// \param x x-pos of event.
		/// \param y y-pos of event.
		/// \param button Button code pressed.
		///
		MouseReleasedEvent(const int x, const int y, const unsigned int button) noexcept;

		///
		/// Copy constructor.
		///
		MouseReleasedEvent(const MouseReleasedEvent&) noexcept = default;

		///
		/// Move constructor.
		///
		MouseReleasedEvent(MouseReleasedEvent&&) noexcept = default;

		///
		/// Copy assignment operator.
		///
		MouseReleasedEvent& operator=(const MouseReleasedEvent&) noexcept = default;

		///
		/// Move assignment operator.
		///
		MouseReleasedEvent& operator=(MouseReleasedEvent&&) noexcept = default;

		///
		/// Default destructor.
		///
		~MouseReleasedEvent() noexcept = default;

		///
		/// Mouse x position.
		///
		int m_x;

		///
		/// Mouse y position.
		///
		int m_y;

		///
		/// Mouse button released.
		///
		unsigned int m_button;
	};
} // namespace pr

#endif