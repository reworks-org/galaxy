///
/// Transform.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
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
		:m_originPoint(0.0f, 0.0f, 0.0f), m_rotateMatrix(1.0f), m_scaledMatrix(1.0f), m_translationMatrix(1.0f), m_modelMatrix(1.0f)
	{
	}

	void Transform::move(const float x, const float y)
	{
		m_translationMatrix = glm::translate(m_translationMatrix, glm::vec3(x, y, 0.0f));
		recalculate();
	}

	void Transform::setPos(const float x, const float y)
	{
		m_translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
		recalculate();
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

		//m_rotateMatrix = glm::translate(glm::mat4(1.0f), m_originPoint);
		m_rotateMatrix = glm::rotate(m_rotateMatrix, glm::radians(adjusted), glm::vec3(0.0f, 0.0f, 1.0f));
		//m_rotateMatrix = glm::translate(m_rotateMatrix, -m_originPoint);
		
		recalculate();
	}

	void Transform::scale(const float scale) noexcept
	{
		m_scaledMatrix = glm::scale(m_scaledMatrix, glm::vec3(scale, scale, 1.0f));
		recalculate();
	}

	void Transform::recalculate() noexcept
	{
		m_modelMatrix = m_translationMatrix * m_rotateMatrix * m_scaledMatrix;
	}

	void Transform::reset() noexcept
	{
		m_translationMatrix = glm::mat4(1.0f);
		m_rotateMatrix = glm::mat4(1.0f);
		m_scaledMatrix = glm::mat4(1.0f);
		m_modelMatrix = glm::mat4(1.0f);
	}

	glm::mat4& Transform::getTransformation() noexcept
	{
		return m_modelMatrix;
	}

	decltype(auto) Transform::getMatrixPtr() noexcept
	{
		return glm::value_ptr(m_modelMatrix);
	}
}