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

const constexpr auto identity_matrix = glm::mat4 {GALAXY_IDENTITY_MATRIX};
const constexpr auto rotation_origin = glm::vec3 {0, 0, 1};

namespace galaxy
{
	namespace graphics
	{
		Camera::Camera(bool allow_rotate)
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

		Camera::Camera(const nlohmann::json& json)
			: m_data {}
		{
			set_projection(0.0f, core::ServiceLocator<core::Window>::ref().get_widthf(), core::ServiceLocator<core::Window>::ref().get_heightf(), 0.0f);

			deserialize(json);
		}

		Camera::Camera(Camera&& c)
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

		Camera& Camera::operator=(Camera&& c)
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

		Camera::Camera(const Camera& c)
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

		Camera& Camera::operator=(const Camera& c)
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

		Camera::~Camera()
		{
		}

		void Camera::on_key_down(events::KeyDown& e)
		{
			if (!e.handled)
			{
				if (e.keycode == input::CameraKeys::FORWARD)
				{
					m_pos.x -= -glm::sin(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
					m_pos.y -= glm::cos(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
				}

				if (e.keycode == input::CameraKeys::BACKWARD)
				{
					m_pos.x += -glm::sin(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
					m_pos.y += glm::cos(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
				}

				if (e.keycode == input::CameraKeys::LEFT)
				{
					m_pos.x -= glm::cos(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
					m_pos.y -= glm::sin(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
				}

				if (e.keycode == input::CameraKeys::RIGHT)
				{
					m_pos.x += glm::cos(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
					m_pos.y += glm::sin(glm::radians(m_rotation)) * m_translation_speed * GALAXY_DT;
				}

				if (m_allow_rotate)
				{
					if (e.keycode == input::CameraKeys::ROTATE_LEFT)
					{
						m_rotation += m_rotation_speed * GALAXY_DT;
					}

					if (e.keycode == input::CameraKeys::ROTATE_RIGHT)
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

				e.handled = true;
			}
		}

		void Camera::on_mouse_wheel(events::MouseWheel& e)
		{
			if (!e.handled)
			{
				m_zoom -= static_cast<float>(e.yoff) * GALAXY_MIN_CAMERA_ZOOM;
				m_zoom  = std::clamp(m_zoom, GALAXY_MIN_CAMERA_ZOOM, GALAXY_MAX_CAMERA_ZOOM);

				e.handled = true;
			}
		}

		void Camera::on_window_resized(const events::WindowResized& e)
		{
			set_viewport(static_cast<float>(e.width), static_cast<float>(e.height));
		}

		void Camera::set_viewport(const float width, const float height)
		{
			set_projection(0.0f, width, height, 0.0f);
		}

		void Camera::set_pos(const float x, const float y)
		{
			m_pos.x = x;
			m_pos.y = y;
		}

		void Camera::set_rotation(const float degrees)
		{
			m_rotation = degrees;
		}

		void Camera::set_zoom(const float offset)
		{
			m_zoom = offset;
		}

		void Camera::translate(const float x, const float y)
		{
			m_pos.x += x;
			m_pos.y += y;
		}

		float Camera::get_x() const
		{
			return m_pos.x;
		}

		float Camera::get_y() const
		{
			return m_pos.y;
		}

		float Camera::get_rotation() const
		{
			return m_rotation;
		}

		float Camera::get_zoom() const
		{
			return m_zoom;
		}

		const glm::vec2& Camera::get_viewport() const
		{
			return m_viewport;
		}

		const glm::mat4& Camera::get_view()
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
			m_origin.x = right * 0.5f;
			m_origin.y = bottom * 0.5f;

			m_viewport.x = right;
			m_viewport.y = bottom;

			m_data.m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		}

		void Camera::recalculate()
		{
			auto rotation = glm::translate(identity_matrix, m_origin);
			rotation      = glm::rotate(rotation, glm::radians(m_rotation), rotation_origin);
			rotation      = glm::translate(rotation, -m_origin);

			auto scale = glm::translate(identity_matrix, m_origin);
			scale      = glm::scale(scale, {m_zoom, m_zoom, 1.0f});
			scale      = glm::translate(scale, -m_origin);

			const auto transform = glm::translate(identity_matrix, m_pos) * rotation * scale;
			m_data.m_model_view  = glm::inverse(transform);
		}

		nlohmann::json Camera::serialize()
		{
			nlohmann::json json       = "{}"_json;
			json["pos"]["x"]          = m_pos.x;
			json["pos"]["y"]          = m_pos.y;
			json["viewport"]["x"]     = m_viewport.x;
			json["viewport"]["y"]     = m_viewport.y;
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

			const auto& vp = json.at("viewport");
			set_viewport(vp.at("x"), vp.at("y"));

			m_rotation          = json.at("rotation");
			m_zoom              = json.at("zoom");
			m_allow_rotate      = json.at("allow_rotate");
			m_rotation_speed    = json.at("rotation_speed");
			m_translation_speed = json.at("translation_speed");
		}
	} // namespace graphics
} // namespace galaxy
