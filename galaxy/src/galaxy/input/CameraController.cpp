///
/// CameraController.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/trigonometric.hpp>

#include "galaxy/input/Input.hpp"

#include "CameraController.hpp"

namespace galaxy
{
	namespace input
	{
		CameraController::CameraController(graphics::Camera& camera)
			: m_camera {std::ref(camera)}
		{
		}

		CameraController::~CameraController()
		{
		}

		void CameraController::on_key_press(events::KeyPress& e)
		{
			if (!e.handled && e.pressed)
			{
				auto& camera = m_camera.get();

				if (e.keycode == CameraKeys::FORWARD)
				{
					camera.m_pos.x -= -glm::sin(glm::radians(camera.m_rotation)) * camera.m_translation_speed * GALAXY_DT;
					camera.m_pos.y -= glm::cos(glm::radians(camera.m_rotation)) * camera.m_translation_speed * GALAXY_DT;
				}

				if (e.keycode == CameraKeys::BACKWARD)
				{
					camera.m_pos.x += -glm::sin(glm::radians(camera.m_rotation)) * camera.m_translation_speed * GALAXY_DT;
					camera.m_pos.y += glm::cos(glm::radians(camera.m_rotation)) * camera.m_translation_speed * GALAXY_DT;
				}

				if (e.keycode == CameraKeys::LEFT)
				{
					camera.m_pos.x -= glm::cos(glm::radians(camera.m_rotation)) * camera.m_translation_speed * GALAXY_DT;
					camera.m_pos.y -= glm::sin(glm::radians(camera.m_rotation)) * camera.m_translation_speed * GALAXY_DT;
				}

				if (e.keycode == CameraKeys::RIGHT)
				{
					camera.m_pos.x += glm::cos(glm::radians(camera.m_rotation)) * camera.m_translation_speed * GALAXY_DT;
					camera.m_pos.y += glm::sin(glm::radians(camera.m_rotation)) * camera.m_translation_speed * GALAXY_DT;
				}

				if (camera.m_allow_rotation)
				{
					if (e.keycode == CameraKeys::ROTATE_LEFT)
					{
						camera.m_rotation += camera.m_rotation_speed * GALAXY_DT;
					}

					if (e.keycode == CameraKeys::ROTATE_RIGHT)
					{
						camera.m_rotation -= camera.m_rotation_speed * GALAXY_DT;
					}

					if (camera.m_rotation > 180.0f)
					{
						camera.m_rotation -= 360.0f;
					}
					else if (camera.m_rotation <= -180.0f)
					{
						camera.m_rotation += 360.0f;
					}

					camera.set_rotation(camera.m_rotation);
				}

				camera.set_pos(camera.m_pos.x, camera.m_pos.y);

				e.handled = true;
			}
		}

		void CameraController::on_mouse_wheel(events::MouseWheel& e)
		{
			if (!e.handled)
			{
				auto& camera = m_camera.get();
				auto& scale  = camera.get_scale();

				camera.set_scale_vertical(
					std::clamp(static_cast<float>(scale.y - (e.yoff * GALAXY_MIN_CAMERA_ZOOM)), GALAXY_MIN_CAMERA_ZOOM, GALAXY_MAX_CAMERA_ZOOM));

				e.handled = true;
			}
		}
	} // namespace input
} // namespace galaxy
