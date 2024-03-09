///
/// CameraController.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_CAMERACONTROLLER_HPP_
#define GALAXY_GRAPHICS_CAMERACONTROLLER_HPP_

#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/graphics/Camera.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		///
		///
		class CameraController final
		{
		  public:
			///
			/// Event processing method for input events.
			///
			/// \param e Takes in a key down event.
			///
			void on_key_down(events::KeyDown& e);

			///
			/// Event processing method for scroll events.
			///
			/// \param e Takes in a mouse wheel scroll event.
			///
			void on_mouse_wheel(events::MouseWheel& e);

		  public:
			///
			///
			///
			Camera m_camera;
		};
	} // namespace graphics
} // namespace galaxy

#endif
