///
/// Transform.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/utils/Globals.hpp"

#include "Transform.hpp"

namespace galaxy
{
	namespace components
	{
		Transform::Transform() noexcept
			: Serializable {}
			, m_pos {0.0f, 0.0f, 0.0f}
			, m_rotation {0.0f}
			, m_scale {1.0f}
			, m_origin {0.0f, 0.0f, 0.0f}
		{
		}

		Transform::Transform(const nlohmann::json& json)
			: Serializable {}
			, m_pos {0.0f, 0.0f, 0.0f}
			, m_rotation {0.0f}
			, m_scale {1.0f}
			, m_origin {0.0f, 0.0f, 0.0f}
		{
			deserialize(json);
		}

		Transform::Transform(Transform&& t) noexcept
			: Serializable {}
		{
			this->m_pos      = std::move(t.m_pos);
			this->m_rotation = t.m_rotation;
			this->m_scale    = t.m_scale;
			this->m_origin   = std::move(t.m_origin);
		}

		Transform& Transform::operator=(Transform&& t) noexcept
		{
			if (this != &t)
			{
				this->m_pos      = std::move(t.m_pos);
				this->m_rotation = t.m_rotation;
				this->m_scale    = t.m_scale;
				this->m_origin   = std::move(t.m_origin);
			}

			return *this;
		}

		void Transform::translate(const float x, const float y) noexcept
		{
			m_pos.x += x;
			m_pos.y += y;
		}

		void Transform::rotate(const float degrees) noexcept
		{
			m_rotation += degrees;
			m_rotation = std::clamp(m_rotation, 0.0f, 360.0f);
		}

		void Transform::scale(const float factor) noexcept
		{
			m_scale = factor;
		}

		void Transform::set_pos(const float x, const float y) noexcept
		{
			m_pos.x = x;
			m_pos.y = y;
		}

		void Transform::set_rotation(const float degrees) noexcept
		{
			m_rotation = std::clamp(degrees, 0.0f, 360.0f);
		}

		void Transform::set_origin(const float x, const float y) noexcept
		{
			m_origin.x = x;
			m_origin.y = y;
		}

		void Transform::reset() noexcept
		{
			static const constexpr auto identity_matrix = glm::mat4 {GALAXY_IDENTITY_MATRIX};

			m_pos      = {0.0f, 0.0f, 0.0f};
			m_rotation = 0.0f;
			m_scale    = 1.0f;
		}

		glm::vec2 Transform::get_pos() noexcept
		{
			return {m_pos.x, m_pos.y};
		}

		float Transform::get_rotation() const noexcept
		{
			return m_rotation;
		}

		float Transform::get_scale() const noexcept
		{
			return m_scale;
		}

		glm::vec2 Transform::get_origin() noexcept
		{
			return {m_origin.x, m_origin.y};
		}

		glm::mat4 Transform::get_transform()
		{
			static const constexpr auto identity_matrix = glm::mat4 {GALAXY_IDENTITY_MATRIX};
			static const constexpr auto rotation_vec    = glm::vec3 {0.0f, 0.0f, 1.0f};
			static auto scale_vec                       = glm::vec3 {1.0f, 1.0f, 1.0f};

			auto rotation = glm::translate(identity_matrix, m_origin);
			rotation      = glm::rotate(rotation, glm::radians(m_rotation), rotation_vec);
			rotation      = glm::translate(rotation, -m_origin);

			auto scale  = glm::translate(identity_matrix, m_origin);
			scale_vec.x = m_scale;
			scale_vec.y = m_scale;
			scale       = glm::scale(scale, scale_vec);
			scale       = glm::translate(scale, -m_origin);

			return glm::translate(identity_matrix, m_pos) * rotation * scale;
		}

		nlohmann::json Transform::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["pos"]["x"] = m_pos.x;
			json["pos"]["y"] = m_pos.y;
			json["rotation"] = m_rotation;
			json["scale"]    = m_scale;

			return json;
		}

		void Transform::deserialize(const nlohmann::json& json)
		{
			reset();

			const auto& pos = json.at("pos");
			set_pos(pos.at("x"), pos.at("y"));

			rotate(json.at("rotation"));
			scale(json.at("scale"));
		}
	} // namespace components
} // namespace galaxy