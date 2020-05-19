///
/// Transform.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/type_ptr.hpp>

#include "Transform.hpp"

///
/// Core namespace.
///
namespace qs
{
	Transform::Transform() noexcept
		:m_isDirty(false), m_originPoint(0.0f, 0.0f, 0.0f), m_rotateMatrix(1.0f), m_scaledMatrix(1.0f), m_translationMatrix(1.0f), m_modelMatrix(1.0f)
	{
	}

	void Transform::move(const float x, const float y) noexcept
	{
		m_translationMatrix = glm::translate(m_translationMatrix, glm::vec3(x, y, 0.0f));
		m_isDirty = true;
	}

	void Transform::rotate(const float degrees) noexcept
	{
		float adjusted = degrees;
		if (adjusted > 360.0f)
		{
			adjusted = 360.0f;
		}
		else if (adjusted < -360.0f)
		{
			adjusted = -360.0f;
		}

		m_rotateMatrix = glm::translate(m_rotateMatrix, m_originPoint);
		m_rotateMatrix = glm::rotate(m_rotateMatrix, glm::radians(adjusted), glm::vec3(0.0f, 0.0f, 1.0f));
		m_rotateMatrix = glm::translate(m_rotateMatrix, -m_originPoint);

		m_isDirty = true;
	}

	void Transform::scale(const float scale) noexcept
	{
		m_scaledMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f));
		m_isDirty = true;
	}

	void Transform::recalculate() noexcept
	{
		if (m_isDirty)
		{
			m_modelMatrix = m_translationMatrix * m_rotateMatrix * m_scaledMatrix;
			m_isDirty = false;
		}
	}

	void Transform::setPos(const float x, const float y) noexcept
	{
		m_translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
		m_isDirty = true;
	}

	void Transform::setRotationOrigin(const float x, const float y) noexcept
	{
		m_originPoint = glm::vec3(x, y, 0.0f);
	}

	const bool Transform::isDirty() const noexcept
	{
		return m_isDirty;
	}

	glm::mat4& Transform::getTransformation() noexcept
	{
		recalculate();
		return m_modelMatrix;
	}
}