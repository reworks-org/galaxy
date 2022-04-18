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
			, m_dirty {true}
			, m_pos {0.0f, 0.0f, 0.0f}
			, m_rotation {0.0f}
			, m_scale {1.0f, 1.0f, 1.0f}
			, m_origin {0.0f, 0.0f, 0.0f}
			, m_translation_matrix {GALAXY_IDENTITY_MATRIX}
			, m_rotation_matrix {GALAXY_IDENTITY_MATRIX}
			, m_scale_matrix {GALAXY_IDENTITY_MATRIX}
			, m_model {GALAXY_IDENTITY_MATRIX}
		{
		}

		Transform::Transform(const nlohmann::json& json)
			: Serializable {}
			, m_dirty {true}
			, m_pos {0.0f, 0.0f, 0.0f}
			, m_rotation {0.0f}
			, m_scale {1.0f, 1.0f, 1.0f}
			, m_origin {0.0f, 0.0f, 0.0f}
			, m_translation_matrix {GALAXY_IDENTITY_MATRIX}
			, m_rotation_matrix {GALAXY_IDENTITY_MATRIX}
			, m_scale_matrix {GALAXY_IDENTITY_MATRIX}
			, m_model {GALAXY_IDENTITY_MATRIX}
		{
			deserialize(json);
		}

		Transform::Transform(Transform&& t) noexcept
			: Serializable {}
			, m_dirty {true}
			, m_pos {0.0f, 0.0f, 0.0f}
			, m_rotation {0.0f}
			, m_scale {1.0f, 1.0f, 1.0f}
			, m_origin {0.0f, 0.0f, 0.0f}
			, m_translation_matrix {GALAXY_IDENTITY_MATRIX}
			, m_rotation_matrix {GALAXY_IDENTITY_MATRIX}
			, m_scale_matrix {GALAXY_IDENTITY_MATRIX}
			, m_model {GALAXY_IDENTITY_MATRIX}
		{
			this->m_dirty              = t.m_dirty;
			this->m_pos                = std::move(t.m_pos);
			this->m_rotation           = t.m_rotation;
			this->m_scale              = t.m_scale;
			this->m_origin             = std::move(t.m_origin);
			this->m_translation_matrix = std::move(t.m_translation_matrix);
			this->m_rotation_matrix    = std::move(t.m_rotation_matrix);
			this->m_scale_matrix       = std::move(t.m_scale_matrix);
			this->m_model              = std::move(t.m_model);
		}

		Transform& Transform::operator=(Transform&& t) noexcept
		{
			if (this != &t)
			{
				this->m_dirty              = t.m_dirty;
				this->m_pos                = std::move(t.m_pos);
				this->m_rotation           = t.m_rotation;
				this->m_scale              = t.m_scale;
				this->m_origin             = std::move(t.m_origin);
				this->m_translation_matrix = std::move(t.m_translation_matrix);
				this->m_rotation_matrix    = std::move(t.m_rotation_matrix);
				this->m_scale_matrix       = std::move(t.m_scale_matrix);
				this->m_model              = std::move(t.m_model);
			}

			return *this;
		}

		void Transform::translate(const float x, const float y) noexcept
		{
			m_pos.x += x;
			m_pos.y += y;

			m_dirty = true;
		}

		void Transform::rotate(const float degrees) noexcept
		{
			m_rotation += degrees;
			m_rotation = std::clamp(m_rotation, 0.0f, 360.0f);

			m_dirty = true;
		}

		void Transform::scale(const float factor) noexcept
		{
			m_scale.x = std::max(0.0f, factor);
			m_scale.y = m_scale.x;

			m_dirty = true;
		}

		void Transform::set_pos(const float x, const float y) noexcept
		{
			m_pos.x = x;
			m_pos.y = y;

			m_dirty = true;
		}

		void Transform::set_rotation(const float degrees) noexcept
		{
			m_rotation = std::clamp(m_rotation, 0.0f, 360.0f);

			m_dirty = true;
		}

		void Transform::set_origin(const float x, const float y) noexcept
		{
			m_origin.x = x;
			m_origin.y = y;

			m_dirty = true;
		}

		void Transform::reset() noexcept
		{
			static const constexpr auto identity_matrix = glm::mat4 {GALAXY_IDENTITY_MATRIX};

			m_dirty              = true;
			m_pos                = {0.0f, 0.0f, 0.0f};
			m_rotation           = 0.0f;
			m_scale              = {1.0f, 1.0f, 1.0f};
			m_origin             = {0.0f, 0.0f, 0.0f};
			m_translation_matrix = identity_matrix;
			m_rotation_matrix    = identity_matrix;
			m_scale_matrix       = identity_matrix;
			m_model              = identity_matrix;
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
			return m_scale.x;
		}

		glm::vec2 Transform::get_origin() noexcept
		{
			return {m_origin.x, m_origin.y};
		}

		const glm::mat4& Transform::get_transform()
		{
			recalculate();
			return m_model;
		}

		void Transform::recalculate()
		{
			static const constexpr auto identity_matrix = glm::mat4 {GALAXY_IDENTITY_MATRIX};
			static const constexpr auto rotation_vec    = glm::vec3 {0.0f, 0.0f, 1.0f};

			if (m_dirty)
			{
				m_dirty = false;

				m_translation_matrix = glm::translate(identity_matrix, m_pos);

				m_rotation_matrix = identity_matrix;
				m_rotation_matrix = glm::translate(m_rotation_matrix, m_origin);
				m_rotation_matrix = glm::rotate(m_rotation_matrix, glm::radians(m_rotation), rotation_vec);
				m_rotation_matrix = glm::translate(m_rotation_matrix, -m_origin);

				m_scale_matrix = identity_matrix;
				m_scale_matrix = glm::translate(m_scale_matrix, m_origin);
				m_scale_matrix = glm::scale(m_scale_matrix, m_scale);
				m_scale_matrix = glm::translate(m_scale_matrix, -m_origin);

				m_model = m_translation_matrix * m_rotation_matrix * m_scale_matrix;
			}
		}

		nlohmann::json Transform::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["pos"]["x"]    = m_pos.x;
			json["pos"]["y"]    = m_pos.y;
			json["origin"]["x"] = m_origin.x;
			json["origin"]["y"] = m_origin.y;
			json["rotation"]    = m_rotation;
			json["scale"]       = m_scale.x;

			return json;
		}

		void Transform::deserialize(const nlohmann::json& json)
		{
			reset();

			const auto& origin = json.at("origin");
			set_origin(origin.at("x"), origin.at("y"));

			const auto& pos = json.at("pos");
			set_pos(pos.at("x"), pos.at("y"));

			rotate(json.at("rotation"));
			scale(json.at("scale"));
		}
	} // namespace components
} // namespace galaxy