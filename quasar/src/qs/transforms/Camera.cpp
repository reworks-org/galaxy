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
		:m_speed(1.0f), m_projection(glm::mat4(1.0f)), m_vp(glm::mat4(1.0f))
	{
	}

	Camera::Camera(const qs::Window* window, const float speed) noexcept
		:m_speed(speed), m_projection(glm::mat4(1.0f)), m_vp(glm::mat4(1.0f))
	{
		configure(window);
	}

	void Camera::configure(const qs::Window* window) noexcept
	{
		// TODO: Last two are near/far for culling - important for optimization.
		m_projection = glm::ortho(0.0f, static_cast<float>(window->getWidth()), 0.0f, 
			static_cast<float>(window->getHeight()), -1.0f, 1.0f);

		m_vp = m_projection * getTransformation();
	}

	void Camera::onKeyDown(const protostar::KeyDownEvent& e)
	{
		m_curKeyDownEvent = e;
	}

	void Camera::update(const double ts) noexcept
	{
		/*
			move(0.0f, (m_speed * ts) * -1.0f);
			move(0.0f, (m_speed * ts));
			move((m_speed * ts) * -1.0f, 0.0f);
			move((m_speed * ts), 0.0f);
		*/

		m_vp = m_projection * getTransformation();
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