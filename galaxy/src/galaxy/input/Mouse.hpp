///
/// Mouse.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_MOUSE_HPP_
#define GALAXY_INPUT_MOUSE_HPP_

#include <glm/vec2.hpp>

#include "galaxy/input/InputDevice.hpp"

namespace galaxy
{
	namespace input
	{
		///
		/// Physical mouse device and state management.
		///
		class Mouse final : public InputDevice
		{
			friend class core::Window;

		public:
			///
			/// \brief Enum class representing mouse buttons.
			///
			/// Values used are based off of GLFW.
			///
			enum class Buttons : int
			{
				UNKNOWN    = -1,
				BTN_1      = 0,
				BTN_2      = 1,
				BTN_3      = 2,
				BTN_4      = 3,
				BTN_5      = 4,
				BTN_6      = 5,
				BTN_7      = 6,
				BTN_8      = 7,
				BTN_LAST   = BTN_8,
				BTN_LEFT   = BTN_1,
				BTN_RIGHT  = BTN_2,
				BTN_MIDDLE = BTN_3
			};

			///
			/// Destructor.
			///
			virtual ~Mouse() noexcept = default;

			///
			/// \brief Enable sticky mouse.
			///
			/// The pollable state of a nouse button will remain pressed until the state of that button is polled.
			///
			void enable_sticky_mouse() noexcept;

			///
			/// Disable sticky mouse.
			///
			void disable_sticky_mouse() noexcept;

			///
			/// Get last recorded mouse position.
			///
			/// \return XY vector position of cursor.
			///
			[[nodiscard]] glm::dvec2 get_pos() noexcept;

		private:
			///
			/// Constructor.
			///
			Mouse() noexcept;

			///
			/// Move constructor.
			///
			Mouse(Mouse&&) = delete;

			///
			/// Move assignment operator.
			///
			Mouse& operator=(Mouse&&) = delete;

			///
			/// Copy constructor.
			///
			Mouse(const Mouse&) = delete;

			///
			/// Copy assignment operator.
			///
			Mouse& operator=(const Mouse&) = delete;
		};
	} // namespace input
} // namespace galaxy

#endif