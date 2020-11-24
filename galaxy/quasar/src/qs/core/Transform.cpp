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
	Transform::Transform()
	    : m_dirty {false}, m_origin {0.0f, 0.0f, 0.0f}, m_rotation {1.0f}, m_scaling {1.0f}, m_translation {1.0f}, m_model {1.0f}
	{
	}

	Transform::Transform(const Transform& t)
	{
		this->m_dirty       = t.m_dirty;
		this->m_model       = t.m_model;
		this->m_origin      = t.m_origin;
		this->m_rotation    = t.m_rotation;
		this->m_scaling     = t.m_scaling;
		this->m_translation = t.m_translation;
	}

	Transform::Transform(Transform&& t)
	{
		this->m_dirty       = t.m_dirty;
		this->m_model       = std::move(t.m_model);
		this->m_origin      = std::move(t.m_origin);
		this->m_rotation    = std::move(t.m_rotation);
		this->m_scaling     = std::move(t.m_scaling);
		this->m_translation = std::move(t.m_translation);
	}

	Transform& Transform::operator=(const Transform& t)
	{
		this->m_dirty       = t.m_dirty;
		this->m_model       = t.m_model;
		this->m_origin      = t.m_origin;
		this->m_rotation    = t.m_rotation;
		this->m_scaling     = t.m_scaling;
		this->m_translation = t.m_translation;

		return *this;
	}

	Transform& Transform::operator=(Transform&& t)
	{
		if (this != &t)
		{
			this->m_dirty       = t.m_dirty;
			this->m_model       = std::move(t.m_model);
			this->m_origin      = std::move(t.m_origin);
			this->m_rotation    = std::move(t.m_rotation);
			this->m_scaling     = std::move(t.m_scaling);
			this->m_translation = std::move(t.m_translation);
		}

		return *this;
	}

	void Transform::move(const float x, const float y)
	{
		m_translation = glm::translate(m_translation, {x, y, 0.0f});
		m_dirty       = true;
	}

	void Transform::rotate(float degrees)
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

	void Transform::scale(const float scale)
	{
		m_scaling = glm::scale(glm::mat4 {1.0f}, {scale, scale, 1.0f}); // wants mat4 here for some reason?
		m_dirty   = true;
	}

	void Transform::recalculate()
	{
		if (m_dirty)
		{
			m_model = m_translation * m_rotation * m_scaling;
			m_dirty = false;
		}
	}

	void Transform::set_pos(const float x, const float y)
	{
		m_translation = glm::translate(glm::mat4 {1.0f}, {x, y, 0.0f}); // wants mat4 here for some reason?
		m_dirty       = true;
	}

	void Transform::set_rotation_origin(const float x, const float y)
	{
		m_origin = {x, y, 0.0f};
	}

	const bool Transform::is_dirty() const
	{
		return m_dirty;
	}

	glm::mat4& Transform::get_transform()
	{
		recalculate();
		return m_model;
	}
} // namespace qs