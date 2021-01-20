///
/// Transform.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>

#include "Transform.hpp"

namespace galaxy
{
	namespace components
	{
		Transform::Transform() noexcept
		    : m_dirty {true}, m_origin {0.0f, 0.0f, 0.0f}, m_rotation {1.0f}, m_scaling {1.0f}, m_translation {1.0f}, m_model {1.0f}, m_identity_matrix {1.0f}, m_rotate {0.0f}, m_scale {0.0f}, m_pos {0.0f, 0.0f}
		{
		}

		Transform::Transform(const nlohmann::json& json)
		{
			set_pos(json.at("x"), json.at("y"));
			set_rotation_origin(json.at("origin-x"), json.at("origin-y"));
			rotate(json.at("rotation"));
			scale(json.at("scale"));
		}

		Transform::Transform(Transform&& t) noexcept
		{
			this->m_dirty           = t.m_dirty;
			this->m_identity_matrix = std::move(t.m_identity_matrix);
			this->m_model           = std::move(t.m_model);
			this->m_origin          = std::move(t.m_origin);
			this->m_pos             = std::move(t.m_pos);
			this->m_rotate          = t.m_rotate;
			this->m_rotation        = std::move(t.m_rotation);
			this->m_scale           = t.m_scale;
			this->m_scaling         = std::move(t.m_scaling);
			this->m_translation     = std::move(t.m_translation);
		}

		Transform& galaxy::components::Transform::operator=(Transform&& t) noexcept
		{
			if (this != &t)
			{
				this->m_dirty           = t.m_dirty;
				this->m_identity_matrix = std::move(t.m_identity_matrix);
				this->m_model           = std::move(t.m_model);
				this->m_origin          = std::move(t.m_origin);
				this->m_pos             = std::move(t.m_pos);
				this->m_rotate          = t.m_rotate;
				this->m_rotation        = std::move(t.m_rotation);
				this->m_scale           = t.m_scale;
				this->m_scaling         = std::move(t.m_scaling);
				this->m_translation     = std::move(t.m_translation);
			}

			return *this;
		}

		void Transform::move(const float x, const float y) noexcept
		{
			m_translation = glm::translate(m_translation, {x, y, 0.0f});

			m_pos.x += x;
			m_pos.y += y;

			m_dirty = true;
		}

		void Transform::rotate(const float degrees) noexcept
		{
			m_rotate = degrees;
			std::clamp(m_rotate, 0.0f, 360.0f);

			m_rotation = glm::translate(m_rotation, m_origin);
			m_rotation = glm::rotate(m_rotation, glm::radians(m_rotate), {0.0f, 0.0f, 1.0f});
			m_rotation = glm::translate(m_rotation, -m_origin);

			m_dirty = true;
		}

		void Transform::scale(const float scale) noexcept
		{
			m_scaling = glm::scale(m_identity_matrix, {scale, scale, 1.0f});

			m_scale = scale;

			m_dirty = true;
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
			m_translation = glm::translate(m_identity_matrix, {x, y, 0.0f});

			m_pos.x = x;
			m_pos.y = y;

			m_dirty = true;
		}

		void Transform::set_rotation_origin(const float x, const float y) noexcept
		{
			m_origin.x = x;
			m_origin.y = y;
		}

		const bool Transform::is_dirty() const noexcept
		{
			return m_dirty;
		}

		const glm::mat4& Transform::get_transform() noexcept
		{
			recalculate();
			return m_model;
		}

		const float Transform::get_rotation() const noexcept
		{
			return m_rotate;
		}

		const float Transform::get_scale() const noexcept
		{
			return m_scale;
		}

		const glm::vec2& Transform::get_pos() const noexcept
		{
			return m_pos;
		}
	} // namespace components
} // namespace galaxy