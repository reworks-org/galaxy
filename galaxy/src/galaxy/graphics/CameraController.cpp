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
	namespace graphics
	{
		void CameraController::on_key_down(events::KeyDown& e)
		{
			if (!e.handled)
			{
				if (e.keycode == input::CameraKeys::FORWARD)
				{
					m_camera.m_pos.x -= -glm::sin(glm::radians(m_camera.m_rotation)) * m_camera.m_translation_speed * GALAXY_DT;
					m_camera.m_pos.y -= glm::cos(glm::radians(m_camera.m_rotation)) * m_camera.m_translation_speed * GALAXY_DT;
				}

				if (e.keycode == input::CameraKeys::BACKWARD)
				{
					m_camera.m_pos.x += -glm::sin(glm::radians(m_camera.m_rotation)) * m_camera.m_translation_speed * GALAXY_DT;
					m_camera.m_pos.y += glm::cos(glm::radians(m_camera.m_rotation)) * m_camera.m_translation_speed * GALAXY_DT;
				}

				if (e.keycode == input::CameraKeys::LEFT)
				{
					m_camera.m_pos.x -= glm::cos(glm::radians(m_camera.m_rotation)) * m_camera.m_translation_speed * GALAXY_DT;
					m_camera.m_pos.y -= glm::sin(glm::radians(m_camera.m_rotation)) * m_camera.m_translation_speed * GALAXY_DT;
				}

				if (e.keycode == input::CameraKeys::RIGHT)
				{
					m_camera.m_pos.x += glm::cos(glm::radians(m_camera.m_rotation)) * m_camera.m_translation_speed * GALAXY_DT;
					m_camera.m_pos.y += glm::sin(glm::radians(m_camera.m_rotation)) * m_camera.m_translation_speed * GALAXY_DT;
				}

				if (m_camera.m_allow_rotation)
				{
					if (e.keycode == input::CameraKeys::ROTATE_LEFT)
					{
						m_camera.m_rotation += m_camera.m_rotation_speed * GALAXY_DT;
					}

					if (e.keycode == input::CameraKeys::ROTATE_RIGHT)
					{
						m_camera.m_rotation -= m_camera.m_rotation_speed * GALAXY_DT;
					}

					if (m_camera.m_rotation > 180.0f)
					{
						m_camera.m_rotation -= 360.0f;
					}
					else if (m_camera.m_rotation <= -180.0f)
					{
						m_camera.m_rotation += 360.0f;
					}

					m_camera.set_rotation(m_camera.m_rotation);
				}

				m_camera.set_pos(m_camera.m_pos.x, m_camera.m_pos.y);

				e.handled = true;
			}
		}

		void CameraController::on_mouse_wheel(events::MouseWheel& e)
		{
			if (!e.handled)
			{
				auto& scale = m_camera.get_scale();
				m_camera.set_scale_vertical(
					std::clamp(static_cast<float>(scale.y - (e.yoff * GALAXY_MIN_CAMERA_ZOOM)), GALAXY_MIN_CAMERA_ZOOM, GALAXY_MAX_CAMERA_ZOOM));

				e.handled = true;
			}
		}
	} // namespace graphics
} // namespace galaxy
