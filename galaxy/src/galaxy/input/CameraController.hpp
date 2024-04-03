///
/// CameraController.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_CAMERACONTROLLER_HPP_
#define GALAXY_INPUT_CAMERACONTROLLER_HPP_

#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/graphics/Camera.hpp"

namespace galaxy
{
	namespace input
	{
		///
		/// Orthographic camera input wrapper.
		///
		class CameraController final
		{
		  public:
			///
			/// Constructor.
			///
			/// \param camera Reference to camera object to manipulate.
			///
			CameraController(graphics::Camera& camera);

			///
			/// Destructor.
			///
			~CameraController();

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

		  private:
			///
			/// Deleted constructor.
			///
			CameraController();

		  public:
			///
			/// Camera object being manipulated.
			///
			std::reference_wrapper<graphics::Camera> m_camera;
		};
	} // namespace input
} // namespace galaxy

#endif
