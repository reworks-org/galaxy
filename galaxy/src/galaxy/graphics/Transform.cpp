///
/// Transform.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/utils/Globals.hpp"

#include "Transform.hpp"

const constexpr auto identity_matrix = glm::mat4 {GALAXY_IDENTITY_MATRIX};
const constexpr auto rotation_vec    = glm::vec3 {0.0f, 0.0f, 1.0f};

namespace galaxy
{
	namespace graphics
	{
		Transform::Transform()
			: m_pos {0.0f, 0.0f}
			, m_rotation {0.0f}
			, m_scale {1.0f, 1.0f}
			, m_origin {0.0f, 0.0f}
			, m_dirty {true}
			, m_transform {GALAXY_IDENTITY_MATRIX}
		{
		}

		Transform::Transform(Transform&& t)
		{
			this->m_pos       = std::move(t.m_pos);
			this->m_rotation  = t.m_rotation;
			this->m_scale     = std::move(t.m_scale);
			this->m_origin    = std::move(t.m_origin);
			this->m_dirty     = t.m_dirty;
			this->m_transform = std::move(t.m_transform);
		}

		Transform& Transform::operator=(Transform&& t)
		{
			if (this != &t)
			{
				this->m_pos       = std::move(t.m_pos);
				this->m_rotation  = t.m_rotation;
				this->m_scale     = std::move(t.m_scale);
				this->m_origin    = std::move(t.m_origin);
				this->m_dirty     = t.m_dirty;
				this->m_transform = std::move(t.m_transform);
			}

			return *this;
		}

		Transform::Transform(const Transform& t)
		{
			this->m_pos       = t.m_pos;
			this->m_rotation  = t.m_rotation;
			this->m_scale     = t.m_scale;
			this->m_origin    = t.m_origin;
			this->m_dirty     = t.m_dirty;
			this->m_transform = t.m_transform;
		}

		Transform& Transform::operator=(const Transform& t)
		{
			if (this != &t)
			{
				this->m_pos       = t.m_pos;
				this->m_rotation  = t.m_rotation;
				this->m_scale     = t.m_scale;
				this->m_origin    = t.m_origin;
				this->m_dirty     = t.m_dirty;
				this->m_transform = t.m_transform;
			}

			return *this;
		}

		Transform::~Transform()
		{
		}

		void Transform::translate(const float x, const float y)
		{
			m_pos.x += x;
			m_pos.y += y;

			m_dirty = true;
		}

		void Transform::rotate(const float degrees)
		{
			m_rotation += degrees;
			m_rotation  = std::clamp(m_rotation, -360.0f, 360.0f);

			m_dirty = true;
		}

		void Transform::scale(const float scale)
		{
			m_scale.x = scale;
			m_scale.y = scale;

			m_dirty = true;
		}

		void Transform::set_scale_horizontal(const float x)
		{
			m_scale.x = x;
			m_dirty   = true;
		}

		void Transform::set_scale_vertical(const float y)
		{
			m_scale.y = y;
			m_dirty   = true;
		}

		void Transform::set_pos(const float x, const float y)
		{
			m_pos.x = x;
			m_pos.y = y;

			m_dirty = true;
		}

		void Transform::set_rotation(const float degrees)
		{
			m_rotation = std::clamp(degrees, -360.0f, 360.0f);
			m_dirty    = true;
		}

		void Transform::set_origin(const float x, const float y)
		{
			m_origin.x = x;
			m_origin.y = y;

			m_dirty = true;
		}

		void Transform::reset()
		{
			m_pos      = {0.0f, 0.0f};
			m_rotation = 0.0f;
			m_scale    = {1.0f, 1.0f};

			m_dirty = true;
		}

		const glm::vec2& Transform::get_pos() const
		{
			return m_pos;
		}

		float Transform::get_rotation() const
		{
			return m_rotation;
		}

		const glm::vec2& Transform::get_scale() const
		{
			return m_scale;
		}

		const glm::vec2& Transform::get_origin() const
		{
			return m_origin;
		}

		glm::mat4& Transform::get_transform()
		{
			if (m_dirty)
			{
				m_dirty = false;

				const auto origin = glm::vec3 {m_origin, 0.0f};

				auto rotation = glm::translate(identity_matrix, origin);
				rotation      = glm::rotate(rotation, glm::radians(m_rotation), rotation_vec);
				rotation      = glm::translate(rotation, -origin);

				auto scale = glm::translate(identity_matrix, origin);
				scale      = glm::scale(scale, {m_scale, 1.0f});
				scale      = glm::translate(scale, -origin);

				m_transform = glm::translate(identity_matrix, {m_pos, 0.0f}) * rotation * scale;
			}

			return m_transform;
		}
	} // namespace graphics
} // namespace galaxy
