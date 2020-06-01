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
		:m_moveUp(false), m_moveDown(false), m_moveLeft(false), m_moveRight(false), m_speed(1.0f), m_width(0.0f), m_height(0.0f), m_projection(1.0f)
	{
	}

	Camera::Camera(const float left, const float right, const float bottom, const float top, const float speed) noexcept
		:m_moveUp(false), m_moveDown(false), m_moveLeft(false), m_moveRight(false), m_speed(speed), m_width(0.0f), m_height(0.0f), m_projection(1.0f)
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

	void Camera::onKeyDown(const protostar::KeyDownEvent& e) noexcept
	{
		switch (e.m_keycode)
		{
		case protostar::Keys::W:
			m_moveUp = true;
			break;

		case protostar::Keys::S:
			m_moveDown = true;
			break;

		case protostar::Keys::A:
			m_moveLeft = true;
			break;

		case protostar::Keys::D:
			m_moveRight = true;
			break;
		}
	}

	void Camera::onKeyUp(const protostar::KeyUpEvent& e) noexcept
	{
		switch (e.m_keycode)
		{
		case protostar::Keys::W:
			m_moveUp = false;
			break;

		case protostar::Keys::S:
			m_moveDown = false;
			break;

		case protostar::Keys::A:
			m_moveLeft = false;
			break;

		case protostar::Keys::D:
			m_moveRight = false;
			break;
		}
	}

	void Camera::onMouseScroll(const protostar::MouseWheelEvent& e) noexcept
	{
		static double scale = 1.0;
		if (e.m_yOffset < 0)
		{
			scale -= 0.1;
		}
		else
		{
			scale += 0.1;
		}

		this->scale(scale);
	}

	void Camera::update(const double ts) noexcept
	{
		float tsf = static_cast<float>(ts);
		if (m_moveUp)
		{
			move(0.0f, tsf * m_speed);
		}

		if (m_moveDown)
		{
			move(0.0f, (tsf * m_speed) * -1.0f);
		}

		if (m_moveLeft)
		{
			move(tsf * m_speed, 0.0f);
		}

		if (m_moveRight)
		{
			move((tsf * m_speed) * -1.0f, 0.0f);
		}
	}

	void Camera::setSpeed(const float speed) noexcept
	{
		m_speed = speed;
	}

	const float Camera::getWidth() const noexcept
	{
		return m_width;
	}

	const float Camera::getHeight() const noexcept
	{
		return m_height;
	}

	const glm::mat4& Camera::getProj() noexcept
	{
		return m_projection;
	}
}