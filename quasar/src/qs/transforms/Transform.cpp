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
		:m_identityMatrix(glm::mat4(1.0f))
	{
		m_rotateMatrix = m_identityMatrix;
		m_scaledMatrix = m_identityMatrix;
		m_translationMatrix = m_identityMatrix;
		m_modelMatrix = m_identityMatrix;
	}

	void Transform::move(const float x, const float y, const float z)
	{
		m_translationMatrix = glm::translate(m_translationMatrix, glm::vec3(x, y, z));
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

		m_rotateMatrix = glm::rotate(m_rotateMatrix, glm::radians(adjusted), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	void Transform::scale(const float x, const float y, const float z) noexcept
	{
		m_scaledMatrix = glm::scale(m_scaledMatrix, glm::vec3(x, y, z));
	}

	glm::mat4& Transform::getTransformation() noexcept
	{
		m_modelMatrix = m_translationMatrix * m_rotateMatrix * m_scaledMatrix;
		return m_modelMatrix;
	}

	decltype(auto) Transform::getMatrixPtr() noexcept
	{
		return glm::value_ptr(getTransformation());
	}
}