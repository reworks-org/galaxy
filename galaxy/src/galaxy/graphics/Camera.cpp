///
/// Camera.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json.hpp>

#include "Camera.hpp"

namespace galaxy
{
	namespace graphics
	{
		Camera::Camera()
			: m_allow_rotation {false}
			, m_translation_speed {180.0f}
			, m_rotation_speed {180.0f}
		{
		}

		Camera::Camera(const nlohmann::json& json)
			: m_allow_rotation {false}
			, m_translation_speed {180.0f}
			, m_rotation_speed {180.0f}
		{
			deserialize(json);
		}

		Camera::Camera(Camera&& c)
			: Transform {std::move(c)}
		{
			this->m_allow_rotation    = c.m_allow_rotation;
			this->m_translation_speed = c.m_translation_speed;
			this->m_rotation_speed    = c.m_rotation_speed;
			this->m_data.m_model_view = std::move(c.m_data.m_model_view);
			this->m_data.m_projection = std::move(c.m_data.m_projection);
			this->m_viewport          = std::move(c.m_viewport);
		}

		Camera& Camera::operator=(Camera&& c)
		{
			if (this != &c)
			{
				Transform::operator=(std::move(c));

				this->m_allow_rotation    = c.m_allow_rotation;
				this->m_translation_speed = c.m_translation_speed;
				this->m_rotation_speed    = c.m_rotation_speed;
				this->m_data.m_model_view = std::move(c.m_data.m_model_view);
				this->m_data.m_projection = std::move(c.m_data.m_projection);
				this->m_viewport          = std::move(c.m_viewport);
			}

			return *this;
		}

		Camera::Camera(const Camera& c)
			: Transform {c}
		{
			this->m_allow_rotation    = c.m_allow_rotation;
			this->m_translation_speed = c.m_translation_speed;
			this->m_rotation_speed    = c.m_rotation_speed;
			this->m_data.m_model_view = c.m_data.m_model_view;
			this->m_data.m_projection = c.m_data.m_projection;
			this->m_viewport          = c.m_viewport;
		}

		Camera& Camera::operator=(const Camera& c)
		{
			if (this != &c)
			{
				Transform::operator=(c);

				this->m_allow_rotation    = c.m_allow_rotation;
				this->m_translation_speed = c.m_translation_speed;
				this->m_rotation_speed    = c.m_rotation_speed;
				this->m_data.m_model_view = c.m_data.m_model_view;
				this->m_data.m_projection = c.m_data.m_projection;
				this->m_viewport          = c.m_viewport;
			}

			return *this;
		}

		Camera::~Camera()
		{
		}

		void Camera::set_viewport(const float width, const float height)
		{
			set_projection(0.0f, width, height, 0.0f);
		}

		void Camera::translate(const float x, const float y)
		{
			m_pos.x += x * m_translation_speed;
			m_pos.y += y * m_translation_speed;

			m_dirty = true;
		}

		void Camera::rotate(const float degrees)
		{
			m_rotation += degrees * m_rotation_speed;
			m_rotation  = std::clamp(m_rotation, -360.0f, 360.0f);

			m_dirty = true;
		}

		const glm::vec2& Camera::get_viewport() const
		{
			return m_viewport;
		}

		const glm::mat4& Camera::get_model_view()
		{
			recalculate();
			return m_data.m_model_view;
		}

		const glm::mat4& Camera::get_proj()
		{
			recalculate();
			return m_data.m_projection;
		}

		Camera::Data& Camera::get_data()
		{
			recalculate();
			return m_data;
		}

		void Camera::set_projection(const float left, const float right, const float bottom, const float top)
		{
			m_dirty = true;

			m_origin.x = right * 0.5f;
			m_origin.y = bottom * 0.5f;

			m_viewport.x = right;
			m_viewport.y = bottom;

			m_data.m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		}

		void Camera::recalculate()
		{
			if (m_dirty)
			{
				m_data.m_model_view = glm::inverse(get_transform());
				m_dirty             = false;
			}
		}

		nlohmann::json Camera::serialize()
		{
			nlohmann::json json       = "{}"_json;
			json["pos"]["x"]          = m_pos.x;
			json["pos"]["y"]          = m_pos.y;
			json["rotation"]          = m_rotation;
			json["scale"]["x"]        = m_scale.x;
			json["scale"]["y"]        = m_scale.y;
			json["allow_rotation"]    = m_allow_rotation;
			json["rotation_speed"]    = m_rotation_speed;
			json["translation_speed"] = m_translation_speed;

			return json;
		}

		void Camera::deserialize(const nlohmann::json& json)
		{
			const auto& pos = json.at("pos");
			m_pos.x         = pos.at("x");
			m_pos.y         = pos.at("y");

			m_rotation = json.at("rotation");

			const auto& scale = json.at("scale");
			m_scale.x         = scale.at("x");
			m_scale.y         = scale.at("y");

			m_allow_rotation    = json.at("allow_rotation");
			m_rotation_speed    = json.at("rotation_speed");
			m_translation_speed = json.at("translation_speed");

			m_dirty = true;
		}
	} // namespace graphics
} // namespace galaxy
