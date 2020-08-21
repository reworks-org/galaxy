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
	    : m_dirty {false}, m_origin {0.0f, 0.0f, 0.0f}, m_rotation {1.0f}, m_scaling {1.0f}, m_translation {1.0f}, m_model {1.0f}
	{
	}

	void Transform::move(const float x, const float y) noexcept
	{
		m_translation = glm::translate(m_translation, {x, y, 0.0f});
		m_dirty       = true;
	}

	void Transform::rotate(float degrees) noexcept
	{
		if (degrees > 360.0f)
		{
			degrees = 360.0f;
		}

		if (degrees < -360.0f)
		{
			degrees = -360.0f;
		}

		m_rotation = glm::translate(m_rotation, m_origin);
		m_rotation = glm::rotate(m_rotation, glm::radians(degrees), {0.0f, 0.0f, 1.0f});
		m_rotation = glm::translate(m_rotation, -m_origin);

		m_dirty = true;
	}

	void Transform::scale(const float scale) noexcept
	{
		m_scaling = glm::scale(glm::mat4 {1.0f}, {scale, scale, 1.0f}); // wants mat4 here for some reason?
		m_dirty   = true;
	}

	void Transform::recalculate() noexcept
	{
		if (m_dirty)
		{
			m_model = m_translation * m_rotation * m_scaling;
			m_dirty = false;
		}
	}

	void Transform::set_pos(const float x, const float y) noexcept
	{
		m_translation = glm::translate(glm::mat4 {1.0f}, {x, y, 0.0f}); // wants mat4 here for some reason?
		m_dirty       = true;
	}

	void Transform::set_rotation_origin(const float x, const float y) noexcept
	{
		m_origin = {x, y, 0.0f};
	}

	const bool Transform::is_dirty() const noexcept
	{
		return m_dirty;
	}

	glm::mat4& Transform::get_transform() noexcept
	{
		recalculate();
		return m_model;
	}
} // namespace qs