///
/// Camera.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/input/Input.hpp"

#include "Camera.hpp"

namespace galaxy
{
	namespace graphics
	{
		Camera::Camera(bool allow_rotate) noexcept
			: m_allow_rotate {allow_rotate}
			, m_translation_speed {180.0f}
			, m_rotation_speed {180.0f}
			, m_pos {0.0f, 0.0f, 0.0f}
			, m_rotation {0.0f}
			, m_zoom {1.0f}
			, m_data {}
			, m_origin {1.0f, 1.0f, 0.0f}
		{
			set_projection(0.0f, core::ServiceLocator<core::Window>::ref().get_widthf(), core::ServiceLocator<core::Window>::ref().get_heightf(), 0.0f);
		}

		Camera::Camera(const nlohmann::json& json) noexcept
			: m_data {}
		{
			set_projection(0.0f, core::ServiceLocator<core::Window>::ref().get_widthf(), core::ServiceLocator<core::Window>::ref().get_heightf(), 0.0f);

			deserialize(json);
		}

		Camera::Camera(Camera&& c) noexcept
		{
			this->m_allow_rotate      = c.m_allow_rotate;
			this->m_translation_speed = c.m_translation_speed;
			this->m_rotation_speed    = c.m_rotation_speed;
			this->m_pos               = std::move(c.m_pos);
			this->m_rotation          = c.m_rotation;
			this->m_zoom              = c.m_zoom;
			this->m_data.m_model_view = std::move(c.m_data.m_model_view);
			this->m_data.m_projection = std::move(c.m_data.m_projection);
			this->m_origin            = std::move(c.m_origin);
		}

		Camera& Camera::operator=(Camera&& c) noexcept
		{
			if (this != &c)
			{
				this->m_allow_rotate      = c.m_allow_rotate;
				this->m_translation_speed = c.m_translation_speed;
				this->m_rotation_speed    = c.m_rotation_speed;
				this->m_pos               = std::move(c.m_pos);
				this->m_rotation          = c.m_rotation;
				this->m_zoom              = c.m_zoom;
				this->m_data.m_model_view = std::move(c.m_data.m_model_view);
				this->m_data.m_projection = std::move(c.m_data.m_projection);
				this->m_origin            = std::move(c.m_origin);
			}

			return *this;
		}

		Camera::Camera(const Camera& c) noexcept
		{
			this->m_allow_rotate      = c.m_allow_rotate;
			this->m_translation_speed = c.m_translation_speed;
			this->m_rotation_speed    = c.m_rotation_speed;
			this->m_pos               = c.m_pos;
			this->m_rotation          = c.m_rotation;
			this->m_zoom              = c.m_zoom;
			this->m_data.m_model_view = c.m_data.m_model_view;
			this->m_data.m_projection = c.m_data.m_projection;
			this->m_origin            = c.m_origin;
		}

		Camera& Camera::operator=(const Camera& c) noexcept
		{
			if (this != &c)
			{
				this->m_allow_rotate      = c.m_allow_rotate;
				this->m_translation_speed = c.m_translation_speed;
				this->m_rotation_speed    = c.m_rotation_speed;
				this->m_pos               = c.m_pos;
				this->m_rotation          = c.m_rotation;
				this->m_zoom              = c.m_zoom;
				this->m_data.m_model_view = c.m_data.m_model_view;
				this->m_data.m_projection = c.m_data.m_projection;
				this->m_origin            = c.m_origin;
			}

			return *this;
		}

		Camera::~Camera() noexcept
		{
		}

		void Camera::process_events() noexcept
		{
			if (input::Input::key_down(input::CameraKeys::FORWARD))
			{
				m_pos.x -= -glm::sin(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
				m_pos.y -= glm::cos(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
			}

			if (input::Input::key_down(input::CameraKeys::BACKWARD))
			{
				m_pos.x += -glm::sin(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
				m_pos.y += glm::cos(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
			}

			if (input::Input::key_down(input::CameraKeys::LEFT))
			{
				m_pos.x -= glm::cos(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
				m_pos.y -= glm::sin(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
			}

			if (input::Input::key_down(input::CameraKeys::RIGHT))
			{
				m_pos.x += glm::cos(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
				m_pos.y += glm::sin(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
			}

			if (m_allow_rotate)
			{
				if (input::Input::key_down(input::CameraKeys::ROTATE_LEFT))
				{
					m_rotation += m_rotation_speed * GALAXY_DT;
				}

				if (input::Input::key_down(input::CameraKeys::ROTATE_RIGHT))
				{
					m_rotation -= m_rotation_speed * GALAXY_DT;
				}

				if (m_rotation > 180.0f)
				{
					m_rotation -= 360.0f;
				}
				else if (m_rotation <= -180.0f)
				{
					m_rotation += 360.0f;
				}

				set_rotation(m_rotation);
			}

			set_pos(m_pos.x, m_pos.y);
		}

		void Camera::on_mouse_wheel(events::MouseWheel& e) noexcept
		{
			if (!e.m_handled)
			{
				m_zoom -= static_cast<float>(e.m_yoff) * GALAXY_MIN_CAMERA_ZOOM;
				m_zoom = std::clamp(m_zoom, GALAXY_MIN_CAMERA_ZOOM, GALAXY_MAX_CAMERA_ZOOM);

				e.m_handled = true;
			}
		}

		void Camera::on_window_resized(const events::WindowResized& e) noexcept
		{
			set_projection(0.0f, static_cast<float>(e.m_width), static_cast<float>(e.m_height), 0.0f);
		}

		void Camera::set_projection(const float left, const float right, const float bottom, const float top) noexcept
		{
			m_origin.x = right * 0.5f;
			m_origin.y = bottom * 0.5f;

			m_data.m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		}

		void Camera::set_pos(const float x, const float y) noexcept
		{
			m_pos.x = x;
			m_pos.y = y;
		}

		void Camera::set_rotation(const float degrees) noexcept
		{
			m_rotation = degrees;
		}

		void Camera::set_zoom(const float offset) noexcept
		{
			m_zoom = offset;
		}

		float Camera::get_x() const noexcept
		{
			return m_pos.x;
		}

		float Camera::get_y() const noexcept
		{
			return m_pos.y;
		}

		float Camera::get_rotation() const noexcept
		{
			return m_rotation;
		}

		float Camera::get_zoom() const noexcept
		{
			return m_zoom;
		}

		const glm::mat4& Camera::get_view() noexcept
		{
			recalculate();
			return m_data.m_model_view;
		}

		const glm::mat4& Camera::get_proj() noexcept
		{
			recalculate();
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
			json["allow_rotate"]      = m_allow_rotate;
			json["rotation_speed"]    = m_rotation_speed;
			json["translation_speed"] = m_translation_speed;

			return json;
		}

		void Camera::deserialize(const nlohmann::json& json)
		{
			const auto& pos = json.at("pos");
			m_pos.x         = pos.at("x");
			m_pos.y         = pos.at("y");

			m_rotation          = json.at("rotation");
			m_zoom              = json.at("zoom");
			m_allow_rotate      = json.at("allow_rotate");
			m_rotation_speed    = json.at("rotation_speed");
			m_translation_speed = json.at("translation_speed");
		}

		void Camera::recalculate() noexcept
		{
			static const constexpr auto identity_matrix = glm::mat4 {GALAXY_IDENTITY_MATRIX};
			static const constexpr auto rotation_origin = glm::vec3 {0, 0, 1};
			static auto scale_vec                       = glm::vec3 {1.0f, 1.0f, 1.0f};

			auto rotation = glm::translate(identity_matrix, m_origin);
			rotation      = glm::rotate(rotation, glm::radians(m_rotation), rotation_origin);
			rotation      = glm::translate(rotation, -m_origin);

			auto scale  = glm::translate(identity_matrix, m_origin);
			scale_vec.x = m_zoom;
			scale_vec.y = m_zoom;
			scale       = glm::scale(scale, scale_vec);
			scale       = glm::translate(scale, -m_origin);

			const auto transform = glm::translate(identity_matrix, m_pos) * rotation * scale;
			m_data.m_model_view  = glm::inverse(transform);
		}
	} // namespace graphics
} // namespace galaxy