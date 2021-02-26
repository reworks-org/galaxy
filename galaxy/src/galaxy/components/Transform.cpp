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
		    : Serializable {this}, m_dirty {true}, m_rotate {0.0f}, m_pos {0.0f, 0.0f}, m_origin {0.0f, 0.0f, 0.0f}
		{
		}

		Transform::Transform(const nlohmann::json& json)
		    : Serializable {this}, m_dirty {true}, m_rotate {0.0f}, m_pos {0.0f, 0.0f}, m_origin {0.0f, 0.0f, 0.0f}
		{
			deserialize(json);
		}

		Transform::Transform(Transform&& t) noexcept
		    : Serializable {this}
		{
			this->m_dirty       = t.m_dirty;
			this->m_model       = std::move(t.m_model);
			this->m_origin      = std::move(t.m_origin);
			this->m_pos         = std::move(t.m_pos);
			this->m_rotate      = t.m_rotate;
			this->m_rotation    = std::move(t.m_rotation);
			this->m_translation = std::move(t.m_translation);
		}

		Transform& galaxy::components::Transform::operator=(Transform&& t) noexcept
		{
			if (this != &t)
			{
				this->m_dirty       = t.m_dirty;
				this->m_model       = std::move(t.m_model);
				this->m_origin      = std::move(t.m_origin);
				this->m_pos         = std::move(t.m_pos);
				this->m_rotate      = t.m_rotate;
				this->m_rotation    = std::move(t.m_rotation);
				this->m_translation = std::move(t.m_translation);
			}

			return *this;
		}

		void Transform::move(const float x, const float y) noexcept
		{
			m_pos.x += x;
			m_pos.y += y;

			m_dirty = true;
		}

		void Transform::rotate(const float degrees) noexcept
		{
			m_rotate += degrees;
			std::clamp(m_rotate, 0.0f, 360.0f);

			m_dirty = true;
		}

		void Transform::recalculate()
		{
			static const constexpr auto identity_matrix = glm::mat4 {1.0f};

			if (m_dirty)
			{
				m_translation = glm::translate(identity_matrix, {m_pos.x, m_pos.y, 0.0f});

				m_rotation = glm::translate(m_rotation, m_origin);
				m_rotation = glm::rotate(m_rotation, glm::radians(m_rotate), {0.0f, 0.0f, 1.0f});
				m_rotation = glm::translate(m_rotation, -m_origin);
				m_rotate   = 0.0f;

				m_model = m_translation * m_rotation;
				m_dirty = false;
			}
		}

		void Transform::set_pos(const float x, const float y) noexcept
		{
			m_pos.x = x;
			m_pos.y = y;

			m_dirty = true;
		}

		const bool Transform::is_dirty() const noexcept
		{
			return m_dirty;
		}

		const glm::mat4& Transform::get_transform(const float half_width, const float half_height)
		{
			if (half_width >= 0.0f)
			{
				m_origin.x = half_width;
			}

			if (half_height >= 0.0f)
			{
				m_origin.y = half_height;
			}

			recalculate();
			return m_model;
		}

		const float Transform::get_rotation() const noexcept
		{
			return m_rotate;
		}

		const glm::vec2& Transform::get_pos() const noexcept
		{
			return m_pos;
		}

		const glm::vec3& Transform::get_origin() const noexcept
		{
			return m_origin;
		}

		nlohmann::json Transform::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["x"]           = m_pos.x;
			json["y"]           = m_pos.y;
			json["rotation"]    = m_rotate;

			return json;
		}

		void Transform::deserialize(const nlohmann::json& json)
		{
			if ((json.count("x") > 0) && json.count("y") > 0)
			{
				set_pos(json.at("x"), json.at("y"));
			}

			if (json.count("rotation") > 0)
			{
				rotate(json.at("rotation"));
			}
		}
	} // namespace components
} // namespace galaxy