///
/// Transform2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include <glm/gtc/type_ptr.hpp>

#include "Transform2D.hpp"

namespace galaxy
{
	namespace components
	{
		Transform2D::Transform2D() noexcept
			: Serializable {this}
			, m_dirty {true}
			, m_origin {0.0f, 0.0f}
			, m_translation {1.0f}
			, m_rotation {1.0f}
			, m_scale {1.0f}
			, m_pos {0.0f, 0.0f}
			, m_rotate {0.0f}
			, m_scale_factor {1.0f}
			, m_model {1.0f}
		{
		}

		Transform2D::Transform2D(const nlohmann::json& json)
			: Serializable {this}
			, m_dirty {true}
			, m_origin {0.0f, 0.0f}
			, m_translation {1.0f}
			, m_rotation {1.0f}
			, m_scale {1.0f}
			, m_pos {0.0f, 0.0f}
			, m_rotate {0.0f}
			, m_scale_factor {1.0f}
			, m_model {1.0f}
		{
			deserialize(json);
		}

		Transform2D::Transform2D(Transform2D&& t) noexcept
			: Serializable {this}
		{
			this->m_dirty        = t.m_dirty;
			this->m_model        = std::move(t.m_model);
			this->m_origin       = std::move(t.m_origin);
			this->m_pos          = std::move(t.m_pos);
			this->m_rotate       = t.m_rotate;
			this->m_rotation     = std::move(t.m_rotation);
			this->m_scale        = std::move(t.m_scale);
			this->m_scale_factor = t.m_scale_factor;
			this->m_translation  = std::move(t.m_translation);
		}

		Transform2D& Transform2D::operator=(Transform2D&& t) noexcept
		{
			if (this != &t)
			{
				this->m_dirty        = t.m_dirty;
				this->m_model        = std::move(t.m_model);
				this->m_origin       = std::move(t.m_origin);
				this->m_pos          = std::move(t.m_pos);
				this->m_rotate       = t.m_rotate;
				this->m_rotation     = std::move(t.m_rotation);
				this->m_scale        = std::move(t.m_scale);
				this->m_scale_factor = t.m_scale_factor;
				this->m_translation  = std::move(t.m_translation);
			}

			return *this;
		}

		void Transform2D::set_pos(const float x, const float y) noexcept
		{
			m_pos.x = x;
			m_pos.y = y;

			m_dirty = true;
		}

		void Transform2D::move(const float x, const float y) noexcept
		{
			m_pos.x += x;
			m_pos.y += y;

			m_dirty = true;
		}

		void Transform2D::rotate(const float degrees) noexcept
		{
			m_rotate += degrees;
			m_rotate = std::clamp(m_rotate, 0.0f, 360.0f);

			m_dirty = true;
		}

		void Transform2D::scale(const float factor) noexcept
		{
			m_scale_factor = std::clamp(factor, 0.1f, 2.0f);

			m_dirty = true;
		}

		void Transform2D::set_origin(const float x, const float y) noexcept
		{
			m_origin.x = x;
			m_origin.y = y;

			m_dirty = true;
		}

		void Transform2D::recalculate()
		{
			constexpr const auto identity_matrix = glm::mat4 {1.0f};

			if (m_dirty)
			{
				m_translation = glm::translate(identity_matrix, {m_pos.x, m_pos.y, 0.0f});

				m_rotation = identity_matrix;
				m_rotation = glm::translate(m_rotation, glm::vec3(m_origin, 0.0f));
				m_rotation = glm::rotate(m_rotation, glm::radians(m_rotate), {0.0f, 0.0f, 1.0f});
				m_rotation = glm::translate(m_rotation, -glm::vec3(m_origin, 0.0f));

				m_scale = identity_matrix;
				m_scale = glm::translate(m_scale, glm::vec3(m_origin, 0.0f));
				m_scale = glm::scale(m_scale, {m_scale_factor, m_scale_factor, 1.0f});
				m_scale = glm::translate(m_scale, -glm::vec3(m_origin, 0.0f));

				m_model = m_translation * m_rotation * m_scale;
				m_dirty = false;
			}
		}

		const bool Transform2D::is_dirty() const noexcept
		{
			return m_dirty;
		}

		const glm::mat4& Transform2D::get_transform()
		{
			recalculate();
			return m_model;
		}

		const glm::vec2& Transform2D::get_pos() const noexcept
		{
			return m_pos;
		}

		const float Transform2D::get_rotation() const noexcept
		{
			return m_rotate;
		}

		const float Transform2D::get_scale() const noexcept
		{
			return m_scale_factor;
		}

		const glm::vec2& Transform2D::get_origin() const noexcept
		{
			return m_origin;
		}

		void Transform2D::reset() noexcept
		{
			m_dirty        = true;
			m_origin       = {0.0f, 0.0f};
			m_translation  = glm::mat4 {1.0f};
			m_rotation     = glm::mat4 {1.0f};
			m_scale        = glm::mat4 {1.0f};
			m_pos          = {0.0f, 0.0f};
			m_rotate       = 0.0f;
			m_scale_factor = 1.0f;
			m_model        = glm::mat4 {1.0f};
		}

		nlohmann::json Transform2D::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["x"]           = m_pos.x;
			json["y"]           = m_pos.y;
			json["rotation"]    = m_rotate;
			json["scale"]       = m_scale_factor;

			return json;
		}

		void Transform2D::deserialize(const nlohmann::json& json)
		{
			reset();

			set_pos(json.at("x"), json.at("y"));
			rotate(json.at("rotation"));
			scale(json.at("scale"));
		}
	} // namespace components
} // namespace galaxy