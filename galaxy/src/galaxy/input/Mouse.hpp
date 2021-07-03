///
/// Mouse.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_MOUSE_HPP_
#define GALAXY_INPUT_MOUSE_HPP_

#include <array>
#include <robin_hood.h>

#include "galaxy/input/MouseButtons.hpp"

namespace galaxy
{
	namespace input
	{
		///
		/// Physical mouse device and state management.
		///
		class Mouse final
		{
		public:
			///
			/// Constructor.
			///
			Mouse() noexcept;

			///
			/// Move constructor.
			///
			Mouse(Mouse&&) noexcept;

			///
			/// Move assignment operator.
			///
			Mouse& operator=(Mouse&&) noexcept;

			///
			/// Copy constructor.
			///
			Mouse(const Mouse&) noexcept;

			///
			/// Copy assignment operator.
			///
			Mouse& operator=(const Mouse&) noexcept;

			///
			/// Destructor.
			///
			~Mouse() noexcept;

		public:
			///
			/// Map of galaxy mouse buttons to GLFW mouse buttons.
			///
			robin_hood::unordered_flat_map<MouseButtons, int> m_mouse_map;

			///
			/// Map of GLFW mouse buttons to galaxy mouse buttons.
			///
			robin_hood::unordered_flat_map<int, MouseButtons> m_reverse_mouse_map;

			///
			/// Previous mouse button states.
			///
			std::array<int, 8> m_prev_mouse_btn_states;

			///
			/// Scroll callback value.
			///
			double m_scroll_delta;
		};
	} // namespace input
} // namespace galaxy

#endif