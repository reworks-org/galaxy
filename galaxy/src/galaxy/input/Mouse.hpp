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
			/// Destructor.
			///
			virtual ~Mouse() = default;

			///
			/// \brief Enable sticky mouse.
			///
			/// The pollable state of a nouse button will remain pressed until the state of that button is polled.
			///
			void enable_sticky_mouse() const;

			///
			/// Disable sticky mouse.
			///
			void disable_sticky_mouse() const;

			///
			/// Get last recorded mouse position.
			///
			/// \return XY vector position of cursor.
			///
			[[nodiscard]] glm::dvec2 get_pos();

		private:
			///
			/// Constructor.
			///
			Mouse();

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