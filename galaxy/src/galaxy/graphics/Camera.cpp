///
/// Camera.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Camera.hpp"

namespace galaxy
{
	namespace graphics
	{
		Camera::Camera()
		    : m_scale {0.0}, m_move_up {false}, m_move_down {false}, m_move_left {false}, m_move_right {false}, m_speed {1.0f}, m_width {0.0f}, m_height {0.0f}, m_projection {1.0f}
		{
		}

		Camera::Camera(const float left, const float right, const float bottom, const float top, const float speed)
		    : m_scale {0.0}, m_move_up {false}, m_move_down {false}, m_move_left {false}, m_move_right {false}, m_speed(speed), m_width {0.0f}, m_height {0.0f}, m_projection {1.0f}
		{
			create(left, right, bottom, top);
		}

		void Camera::create(const float left, const float right, const float bottom, const float top)
		{
			if (right > left)
			{
				m_width = right;
			}
			else
			{
				m_width = left;
			}

			if (bottom > top)
			{
				m_height = bottom;
			}
			else
			{
				m_height = top;
			}

			m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		}

		void Camera::on_key_down(const events::KeyDown& e)
		{
			switch (e.m_keycode)
			{
				case input::Keys::W:
					m_move_up = true;
					break;

				case input::Keys::S:
					m_move_down = true;
					break;

				case input::Keys::A:
					m_move_left = true;
					break;

				case input::Keys::D:
					m_move_right = true;
					break;
			}
		}

		void Camera::on_key_up(const events::KeyUp& e)
		{
			switch (e.m_keycode)
			{
				case input::Keys::W:
					m_move_up = false;
					break;

				case input::Keys::S:
					m_move_down = false;
					break;

				case input::Keys::A:
					m_move_left = false;
					break;

				case input::Keys::D:
					m_move_right = false;
					break;
			}
		}

		void Camera::on_mouse_scroll(const events::MouseWheel& e)
		{
			if (e.m_y_offset < 0)
			{
				m_scale -= 0.1;
			}
			else
			{
				m_scale += 0.1;
			}

			this->scale(static_cast<float>(m_scale));
		}

		void Camera::update(const double ts)
		{
			const float tsf = static_cast<float>(ts);
			if (m_move_up)
			{
				move(0.0f, tsf * m_speed);
			}

			if (m_move_down)
			{
				move(0.0f, (tsf * m_speed) * -1.0f);
			}

			if (m_move_left)
			{
				move(tsf * m_speed, 0.0f);
			}

			if (m_move_right)
			{
				move((tsf * m_speed) * -1.0f, 0.0f);
			}
		}

		void Camera::set_speed(const float speed)
		{
			m_speed = speed;
		}

		const float Camera::get_width() const
		{
			return m_width;
		}

		const float Camera::get_height() const
		{
			return m_height;
		}

		const glm::mat4& Camera::get_proj()
		{
			return m_projection;
		}
	} // namespace graphics
} // namespace galaxy