///
/// MouseEnter.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSEENTER_HPP_
#define GALAXY_EVENTS_MOUSEENTER_HPP_

namespace galaxy
{
	namespace events
	{
		///
		/// Contains data relating to a glfwSetCursorEnterCallback.
		///
		struct MouseEnter final
		{
			///
			/// GLFW_TRUE if the cursor entered the window's content area, or GLFW_FALSE if it left it.
			///
			bool entered = false;

			///
			/// Has this event been handled?
			///
			bool handled = false;
		};
	} // namespace events
} // namespace galaxy

#endif
