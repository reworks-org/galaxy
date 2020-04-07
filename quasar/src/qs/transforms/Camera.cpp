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
		:m_curKeyDownEvent(), m_speed(1.0f), m_projection(1.0f), m_vp(1.0f)
	{
	}

	Camera::Camera(const float left, const float right, const float bottom, const float top, const float speed) noexcept
		:m_curKeyDownEvent(), m_speed(speed), m_projection(1.0f), m_vp(1.0f)
	{
		configure(left, right, bottom, top);
	}

	void Camera::configure(const float left, const float right, const float bottom, const float top) noexcept
	{
		m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_vp = m_projection * getTransformation();
	}

	void Camera::onKeyDown(const protostar::KeyDownEvent& e)
	{
		m_curKeyDownEvent = e;
	}

	void Camera::update(const double ts) noexcept
	{
		switch (m_curKeyDownEvent.m_keycode)
		{
		case protostar::Keys::UP:
			move(0.0f, (ts * m_speed) * -1.0f);
			break;

		case protostar::Keys::DOWN:
			move(0.0f, ts * m_speed);
			break;

		case protostar::Keys::LEFT:
			move(ts * m_speed, 0.0f);
			break;

		case protostar::Keys::RIGHT:
			move((ts * m_speed) * -1.0f, 0.0f);
			break;

		case protostar::Keys::Q:
			rotate(m_speed * ts);
			break;

		case protostar::Keys::E:
			rotate(m_speed * ts);
			break;
		}

		m_vp = m_projection * getTransformation();
		m_curKeyDownEvent.m_keycode = protostar::Keys::F10;
	}

	void Camera::setSpeed(const float speed) noexcept
	{
		m_speed = speed;
	}

	const glm::mat4& Camera::get() const noexcept
	{
		return m_vp;
	}
}