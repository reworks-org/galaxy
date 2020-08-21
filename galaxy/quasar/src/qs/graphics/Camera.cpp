///
/// Camera.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "Camera.hpp"

namespace qs
{
	Camera::Camera() noexcept
	    : m_move_up {false}, m_move_down {false}, m_move_left {false}, m_move_right {false}, m_speed {1.0f}, m_width {0.0f}, m_height {0.0f}, m_projection {1.0f}
	{
	}

	Camera::Camera(const float left, const float right, const float bottom, const float top, const float speed) noexcept
	    : m_move_up {false}, m_move_down {false}, m_move_left {false}, m_move_right {false}, m_speed(speed), m_width {0.0f}, m_height {0.0f}, m_projection {1.0f}
	{
		create(left, right, bottom, top);
	}

	void Camera::create(const float left, const float right, const float bottom, const float top) noexcept
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

	void Camera::on_key_down(const pr::KeyDownEvent& e) noexcept
	{
		switch (e.m_keycode)
		{
			case pr::Keys::W:
				m_move_up = true;
				break;

			case pr::Keys::S:
				m_move_down = true;
				break;

			case pr::Keys::A:
				m_move_left = true;
				break;

			case pr::Keys::D:
				m_move_right = true;
				break;
		}
	}

	void Camera::on_key_up(const pr::KeyUpEvent& e) noexcept
	{
		switch (e.m_keycode)
		{
			case pr::Keys::W:
				m_move_up = false;
				break;

			case pr::Keys::S:
				m_move_down = false;
				break;

			case pr::Keys::A:
				m_move_left = false;
				break;

			case pr::Keys::D:
				m_move_right = false;
				break;
		}
	}

	void Camera::on_mouse_scroll(const pr::MouseWheelEvent& e) noexcept
	{
		static double scale = 1.0;
		if (e.m_y_offset < 0)
		{
			scale -= 0.1;
		}
		else
		{
			scale += 0.1;
		}

		this->scale(static_cast<float>(scale));
	}

	void Camera::update(const double ts) noexcept
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

	void Camera::set_speed(const float speed) noexcept
	{
		m_speed = speed;
	}

	const float Camera::get_width() const noexcept
	{
		return m_width;
	}

	const float Camera::get_height() const noexcept
	{
		return m_height;
	}

	const glm::mat4& Camera::get_proj() noexcept
	{
		return m_projection;
	}
} // namespace qs