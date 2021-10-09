///
/// Transform3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>

#include "Transform3D.hpp"

namespace galaxy
{
	namespace components
	{
		Transform3D::Transform3D() noexcept
		    : Serializable {this}
		{
			reset();
		}

		Transform3D::Transform3D(const nlohmann::json& json)
		    : Serializable {this}
		{
			deserialize(json);
		}

		Transform3D::Transform3D(Transform3D&& t) noexcept
		    : Serializable {this}
		{
			this->m_dirty         = t.m_dirty;
			this->m_origin        = std::move(t.m_origin);
			this->m_rotation      = std::move(t.m_rotation);
			this->m_translation   = std::move(t.m_translation);
			this->m_scale         = std::move(t.m_scale);
			this->m_rotate        = t.m_rotate;
			this->m_rotation_axis = std::move(t.m_rotation_axis);
			this->m_pos           = std::move(t.m_pos);
			this->m_scale_factor  = t.m_scale_factor;
			this->m_model         = std::move(t.m_model);
			this->m_inverse       = std::move(t.m_inverse);
		}

		Transform3D& Transform3D::operator=(Transform3D&& t) noexcept
		{
			if (this != &t)
			{
				this->m_dirty         = t.m_dirty;
				this->m_origin        = std::move(t.m_origin);
				this->m_rotation      = std::move(t.m_rotation);
				this->m_translation   = std::move(t.m_translation);
				this->m_scale         = std::move(t.m_scale);
				this->m_rotate        = t.m_rotate;
				this->m_rotation_axis = std::move(t.m_rotation_axis);
				this->m_pos           = std::move(t.m_pos);
				this->m_scale_factor  = t.m_scale_factor;
				this->m_model         = std::move(t.m_model);
				this->m_inverse       = std::move(t.m_inverse);
			}

			return *this;
		}

		void Transform3D::set_pos(const float x, const float y, const float z) noexcept
		{
			m_pos.x = x;
			m_pos.y = y;
			m_pos.z = z;

			m_dirty = true;
		}

		void Transform3D::move(const float x, const float y, const float z) noexcept
		{
			m_pos.x += x;
			m_pos.y += y;
			m_pos.z += z;

			m_dirty = true;
		}

		void Transform3D::rotate(const float degrees) noexcept
		{
			m_rotate += degrees;
			m_rotate = std::clamp(m_rotate, 0.0f, 360.0f);

			m_dirty = true;
		}

		void Transform3D::scale(const float factor) noexcept
		{
			m_scale_factor = factor;
			std::clamp(m_scale_factor, 0.1f, 2.0f);

			m_dirty = true;
		}

		void Transform3D::set_origin(const float x, const float y, const float z) noexcept
		{
			m_origin.x = x;
			m_origin.y = y;
			m_origin.z = z;

			m_dirty = true;
		}

		void Transform3D::set_rotation_axis(const float x, const float y, const float z) noexcept
		{
			m_rotation_axis.x = x;
			m_rotation_axis.y = y;
			m_rotation_axis.z = z;

			m_dirty = true;
		}

		void Transform3D::recalculate()
		{
			constexpr const auto identity_matrix = glm::mat4 {1.0f};

			if (m_dirty)
			{
				m_translation = glm::translate(identity_matrix, m_pos);

				m_rotation = identity_matrix;
				m_rotation = glm::translate(m_rotation, m_origin);
				m_rotation = glm::rotate(m_rotation, glm::radians(m_rotate), m_rotation_axis);
				m_rotation = glm::translate(m_rotation, -m_origin);

				m_scale = identity_matrix;
				m_scale = glm::translate(m_scale, m_origin);
				m_scale = glm::scale(m_scale, {m_scale_factor, m_scale_factor, m_scale_factor});
				m_scale = glm::translate(m_scale, -m_origin);

				m_model   = m_translation * m_rotation * m_scale;
				m_inverse = glm::transpose(glm::inverse(glm::mat3 {m_model}));

				m_dirty = false;
			}
		}

		const bool Transform3D::is_dirty() const noexcept
		{
			return m_dirty;
		}

		const glm::mat4& Transform3D::get_transform()
		{
			recalculate();
			return m_model;
		}

		const glm::mat3& Transform3D::get_inverse()
		{
			recalculate();
			return m_inverse;
		}

		const float Transform3D::get_rotation() const noexcept
		{
			return m_rotate;
		}

		const glm::vec3& Transform3D::get_pos() const noexcept
		{
			return m_pos;
		}

		const float Transform3D::get_scale() const noexcept
		{
			return m_scale_factor;
		}

		const glm::vec3& Transform3D::get_rotation_axis() const noexcept
		{
			return m_rotation_axis;
		}

		const glm::vec3& Transform3D::get_origin() const noexcept
		{
			return m_origin;
		}

		void Transform3D::reset() noexcept
		{
			m_dirty         = true;
			m_origin        = {0.0f, 0.0f, 0.0f};
			m_rotation      = glm::mat4 {1.0f};
			m_translation   = glm::mat4 {1.0f};
			m_scale         = glm::mat4 {1.0f};
			m_rotate        = 0.0f;
			m_rotation_axis = {0.0f, 0.0f, 1.0f};
			m_pos           = {0.0f, 0.0f, 0.0f};
			m_scale_factor  = 1.0f;
			m_model         = glm::mat4 {1.0f};
			m_inverse       = glm::mat3 {1.0f};
		}

		nlohmann::json Transform3D::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["x"]           = m_pos.x;
			json["y"]           = m_pos.y;
			json["z"]           = m_pos.z;
			json["rotation"]    = m_rotate;
			json["scale"]       = m_scale_factor;

			json["rotation-axis"]      = nlohmann::json::object();
			json["rotation-axis"]["x"] = m_rotation_axis.x;
			json["rotation-axis"]["y"] = m_rotation_axis.y;
			json["rotation-axis"]["z"] = m_rotation_axis.z;

			return json;
		}

		void Transform3D::deserialize(const nlohmann::json& json)
		{
			reset();

			set_pos(json.at("x"), json.at("y"), json.at("z"));
			rotate(json.at("rotation"));
			scale(json.at("scale"));

			const auto& rotation_axis = json.at("rotation-axis");
			set_rotation_axis(rotation_axis.at("x"), rotation_axis.at("y"), rotation_axis.at("z"));
		}
	} // namespace components
} // namespace galaxy