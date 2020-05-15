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
		:m_isDirty(true), m_defaultSet(false), m_textureTransform(0.0f, 0.0f, 1.0f), m_originPoint(0.0f, 0.0f, 0.0f), m_rotateMatrix(1.0f), m_scaledMatrix(1.0f), m_translationMatrix(1.0f), m_modelMatrix(1.0f)
	{
	}

	void Transform::move(const float x, const float y) noexcept
	{
		m_translationMatrix = glm::translate(m_translationMatrix, glm::vec3(x, y, 0.0f));
		recalculate();
	}

	void Transform::setPos(const float x, const float y) noexcept
	{
		m_translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
		recalculate();
	}

	void Transform::setRotationOrigin(const float x, const float y) noexcept
	{
		m_originPoint = glm::vec3(x, y, 0.0f);
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
		m_isDirty = true;
	}

	void Transform::setDirty(const bool isDirty) noexcept
	{
		m_isDirty = isDirty;
	}

	void Transform::setOpacity(float opacity) noexcept
	{
		if (opacity > 1.0f)
		{
			opacity = 1.0f;
		}
		else if (opacity < 0.0f)
		{
			opacity = 0.0f;
		}

		m_textureTransform.z = opacity;
		m_isDirty = true;
	}

	void Transform::setTexels(const float x, const float y) noexcept
	{
		m_textureTransform.x = x;
		m_textureTransform.y = y;
		m_isDirty = true;
	}

	void Transform::setDefaultTexels(const float x, const float y) noexcept
	{
		if (!m_defaultSet)
		{
			setTexels(x, y);
			m_defaultSet = true;
		}
	}

	void Transform::moveTexels(const float x, const float y) noexcept
	{
		m_textureTransform.x += x;
		m_textureTransform.y += y;
		m_isDirty = true;
	}

	glm::mat4& Transform::getTransformation() noexcept
	{
		return m_modelMatrix;
	}

	glm::vec3& Transform::getTexelTransform() noexcept
	{
		return m_textureTransform;
	}

	const bool Transform::isDirty() const noexcept
	{
		return m_isDirty;
	}

	const bool Transform::isDefaultSet() const noexcept
	{
		return m_defaultSet;
	}
}