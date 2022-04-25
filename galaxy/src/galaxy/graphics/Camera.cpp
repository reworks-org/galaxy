///
/// Camera.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"

#include "Camera.hpp"

namespace galaxy
{
	namespace graphics
	{
		Camera::Camera() noexcept
			: m_dirty {true}
			, m_data {}
			, m_pos {0.0f, 0.0f, 0.0f}
			, m_rotation {0.0f}
			, m_zoom {1.0f}
			, m_translation_speed {1.0f}
			, m_rotation_speed {1.0f}
			, m_forward_key {input::Keys::W}
			, m_back_key {input::Keys::S}
			, m_left_key {input::Keys::A}
			, m_right_key {input::Keys::D}
			, m_moving_fwd {false}
			, m_moving_back {false}
			, m_moving_left {false}
			, m_moving_right {false}
			, m_transform_matrix {GALAXY_IDENTITY_MATRIX}
			, m_identity_matrix {GALAXY_IDENTITY_MATRIX}
			, m_rotation_origin {0, 0, 1}
		{
			auto& window   = core::ServiceLocator<core::Window>::ref();
			m_aspect_ratio = static_cast<float>(window.get_width()) / static_cast<float>(window.get_height());
		}

		Camera::Camera(const float left, const float right, const float bottom, const float top) noexcept
			: m_dirty {true}
			, m_data {}
			, m_pos {0.0f, 0.0f, 0.0f}
			, m_rotation {0.0f}
			, m_zoom {1.0f}
			, m_translation_speed {1.0f}
			, m_rotation_speed {1.0f}
			, m_forward_key {input::Keys::W}
			, m_back_key {input::Keys::S}
			, m_left_key {input::Keys::A}
			, m_right_key {input::Keys::D}
			, m_moving_fwd {false}
			, m_moving_back {false}
			, m_moving_left {false}
			, m_moving_right {false}
			, m_transform_matrix {GALAXY_IDENTITY_MATRIX}
			, m_identity_matrix {GALAXY_IDENTITY_MATRIX}
			, m_rotation_origin {0, 0, 1}
		{
			auto& window   = core::ServiceLocator<core::Window>::ref();
			m_aspect_ratio = static_cast<float>(window.get_width()) / static_cast<float>(window.get_height());

			create(left, right, bottom, top);
		}

		Camera::Camera(const float aspect_ratio) noexcept
			: m_dirty {true}
			, m_data {}
			, m_pos {0.0f, 0.0f, 0.0f}
			, m_rotation {0.0f}
			, m_zoom {1.0f}
			, m_translation_speed {1.0f}
			, m_rotation_speed {1.0f}
			, m_aspect_ratio {aspect_ratio}
			, m_forward_key {input::Keys::W}
			, m_back_key {input::Keys::S}
			, m_left_key {input::Keys::A}
			, m_right_key {input::Keys::D}
			, m_moving_fwd {false}
			, m_moving_back {false}
			, m_moving_left {false}
			, m_moving_right {false}
			, m_transform_matrix {GALAXY_IDENTITY_MATRIX}
			, m_identity_matrix {GALAXY_IDENTITY_MATRIX}
			, m_rotation_origin {0, 0, 1}
		{
			create(-m_aspect_ratio * m_zoom, m_aspect_ratio * m_zoom, -m_zoom, m_zoom);
		}

		Camera::Camera(const nlohmann::json& json) noexcept
			: m_data {}
			, m_forward_key {input::Keys::W}
			, m_back_key {input::Keys::S}
			, m_left_key {input::Keys::A}
			, m_right_key {input::Keys::D}
			, m_moving_fwd {false}
			, m_moving_back {false}
			, m_moving_left {false}
			, m_moving_right {false}
			, m_transform_matrix {GALAXY_IDENTITY_MATRIX}
			, m_identity_matrix {GALAXY_IDENTITY_MATRIX}
			, m_rotation_origin {0, 0, 1}
		{
			deserialize(json);
		}

		void Camera::on_event(const events::KeyDown& e) noexcept
		{
			if (e.m_keycode == m_forward_key)
			{
				m_moving_fwd = true;
			}

			if (e.m_keycode == m_back_key)
			{
				m_moving_back = true;
			}

			if (e.m_keycode == m_left_key)
			{
				m_moving_left = true;
			}

			if (e.m_keycode == m_right_key)
			{
				m_moving_right = true;
			}
		}

		void Camera::on_event(const events::KeyUp& e) noexcept
		{
			if (e.m_keycode == m_forward_key)
			{
				m_moving_fwd = false;
			}

			if (e.m_keycode == m_back_key)
			{
				m_moving_back = false;
			}

			if (e.m_keycode == m_left_key)
			{
				m_moving_left = false;
			}

			if (e.m_keycode == m_right_key)
			{
				m_moving_right = false;
			}
		}

		void Camera::on_event(const events::MouseWheel& e) noexcept
		{
			m_dirty = true;

			m_zoom -= e.m_yoff * 0.25f;
			m_zoom = std::max(m_zoom, 0.25f);

			set_projection(-m_aspect_ratio * m_zoom, m_aspect_ratio * m_zoom, -m_zoom, m_zoom);
		}

		void Camera::on_event(const events::WindowResized& e) noexcept
		{
			m_aspect_ratio = static_cast<float>(e.m_width) / static_cast<float>(e.m_height);
			create(-m_aspect_ratio * m_zoom, m_aspect_ratio * m_zoom, -m_zoom, m_zoom);
		}

		void Camera::update() noexcept
		{
			if (m_moving_fwd)
			{
				m_pos.x += -glm::sin(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
				m_pos.y += glm::cos(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
			}

			if (m_moving_back)
			{
				m_pos.x -= -glm::sin(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
				m_pos.y -= glm::cos(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
			}

			if (m_moving_left)
			{
				m_pos.x -= glm::cos(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
				m_pos.y -= glm::sin(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
			}

			if (m_moving_right)
			{
				m_pos.x += glm::cos(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
				m_pos.y += glm::sin(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
			}
		}

		void Camera::set_pos(const float x, const float y) noexcept
		{
			m_pos.x = x;
			m_pos.y = y;

			m_dirty = true;
		}

		void Camera::set_rotation(const float degrees) noexcept
		{
			m_rotation = std::clamp(m_rotation, 0.0f, 360.0f);

			m_dirty = true;
		}

		void Camera::set_zoom(const float offset) noexcept
		{
			m_zoom = offset;
		}

		void Camera::set_projection(const float left, const float right, const float bottom, const float top) noexcept
		{
			create(left, right, bottom, top);
		}

		void Camera::set_translation_speed(const float speed) noexcept
		{
			m_translation_speed = speed;
		}

		void Camera::set_rotation_speed(const float speed) noexcept
		{
			m_rotation_speed = speed;
		}

		const glm::mat4& Camera::get_view() noexcept
		{
			recalculate();
			return m_data.m_model_view;
		}

		const glm::mat4& Camera::get_proj() noexcept
		{
			return m_data.m_projection;
		}

		Camera::Data& Camera::get_data() noexcept
		{
			recalculate();
			return m_data;
		}

		nlohmann::json Camera::serialize()
		{
			nlohmann::json json       = "{}"_json;
			json["pos"]["x"]          = m_pos.x;
			json["pos"]["y"]          = m_pos.y;
			json["rotation"]          = m_rotation;
			json["zoom"]              = m_zoom;
			json["translation_speed"] = m_translation_speed;
			json["rotation_speed"]    = m_rotation_speed;
			json["aspect_ratio"]      = m_aspect_ratio;

			return json;
		}

		void Camera::deserialize(const nlohmann::json& json)
		{
			m_dirty = true;

			const auto& pos = json.at("pos");
			m_pos.x         = pos.at("x");
			m_pos.y         = pos.at("y");
			m_pos.z         = 0.0f;

			m_rotation          = json.at("rotation");
			m_zoom              = json.at("zoom");
			m_translation_speed = json.at("translation_speed");
			m_rotation_speed    = json.at("rotation_speed");
			m_aspect_ratio      = json.at("aspect_ratio");

			create(-m_aspect_ratio * m_zoom, m_aspect_ratio * m_zoom, -m_zoom, m_zoom);
		}

		void Camera::create(const float left, const float right, const float bottom, const float top) noexcept
		{
			m_data.m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

			m_dirty = true;
		}

		void Camera::recalculate() noexcept
		{
			if (m_dirty)
			{
				m_dirty = false;

				m_transform_matrix  = glm::translate(m_identity_matrix, m_pos) * glm::rotate(m_identity_matrix, glm::radians(m_rotation), m_rotation_origin);
				m_data.m_model_view = m_data.m_projection * glm::inverse(m_transform_matrix);
			}
		}
	} // namespace graphics
} // namespace galaxy