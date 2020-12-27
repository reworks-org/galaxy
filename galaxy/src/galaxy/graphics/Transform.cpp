///
/// Transform.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/type_ptr.hpp>

#include "Transform.hpp"

namespace galaxy
{
	namespace graphics
	{
		Transform::Transform()
		    : m_dirty {false}, m_origin {0.0f, 0.0f, 0.0f}, m_rotation {1.0f}, m_scaling {1.0f}, m_translation {1.0f}, m_model {1.0f}, m_rotate {0.0f}, m_scale {0.0f}, m_pos {0.0f, 0.0f}
		{
		}

		void Transform::move(const float x, const float y)
		{
			m_translation = glm::translate(m_translation, {x, y, 0.0f});
			m_dirty       = true;
		}

		void Transform::rotate(float degrees)
		{
			m_rotate = degrees;

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
			m_scale   = scale;

			m_dirty = true;
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
			m_pos.x       = x;
			m_pos.y       = y;

			m_dirty = true;
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

		const float Transform::get_rotation() const
		{
			return m_rotate;
		}

		const float Transform::get_scale() const
		{
			return m_scale;
		}

		const glm::vec2& Transform::get_pos() const
		{
			return m_pos;
		}
	} // namespace graphics
} // namespace galaxy