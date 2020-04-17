///
/// Camera.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "Camera.hpp"

namespace qs
{
	Camera::Camera() noexcept
		:m_moveUp(false), m_moveDown(false), m_moveLeft(false), m_moveRight(false), m_speed(1.0f), m_projection(1.0f)
	{
	}

	Camera::Camera(const float left, const float right, const float bottom, const float top, const float speed) noexcept
		:m_moveUp(false), m_moveDown(false), m_moveLeft(false), m_moveRight(false), m_speed(speed), m_projection(1.0f)
	{
		create(left, right, bottom, top);
	}

	void Camera::create(const float left, const float right, const float bottom, const float top) noexcept
	{
		m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	}

	void Camera::onKeyDown(const protostar::KeyDownEvent& e)
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

	void Camera::onKeyUp(const protostar::KeyUpEvent& e)
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

	void Camera::update(const double ts) noexcept
	{
		if (m_moveUp)
		{
			move(0.0f, ts * m_speed);
		}

		if (m_moveDown)
		{
			move(0.0f, (ts * m_speed) * -1.0f);
		}

		if (m_moveLeft)
		{
			move(ts * m_speed, 0.0f);
		}

		if (m_moveRight)
		{
			move((ts * m_speed) * -1.0f, 0.0f);
		}
	}

	void Camera::setSpeed(const float speed) noexcept
	{
		m_speed = speed;
	}

	const glm::mat4& Camera::getProj() noexcept
	{
		return m_projection;
	}
}