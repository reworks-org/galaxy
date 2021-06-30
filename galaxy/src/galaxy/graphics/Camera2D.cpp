///
/// Camera2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json.hpp>

#include "Camera2D.hpp"

#define ORTHO_NEAR      0.0f
#define ORTHO_FAR_24BIT 16777215.0f

namespace galaxy
{
	namespace graphics
	{
		Camera2D::Camera2D() noexcept
		    : Serializable {this}, m_forward_key {input::Keys::W}, m_back_key {input::Keys::S}, m_left_key {input::Keys::A}, m_right_key {input::Keys::D}, m_moving_fwd {false}, m_moving_back {false}, m_moving_left {false}, m_moving_right {false}, m_speed {1.0f}, m_dirty {true}, m_scaling {1.0f}, m_translation {1.0f}, m_identity_matrix {1.0f}, m_scale {1.0f}, m_pos {0.0f, 0.0f}, m_width {1}, m_height {1}
		{
		}

		Camera2D::Camera2D(const nlohmann::json& json) noexcept
		    : Serializable {this}, m_forward_key {input::Keys::W}, m_back_key {input::Keys::S}, m_left_key {input::Keys::A}, m_right_key {input::Keys::D}, m_moving_fwd {false}, m_moving_back {false}, m_moving_left {false}, m_moving_right {false}, m_speed {1.0f}, m_dirty {true}, m_scaling {1.0f}, m_translation {1.0f}, m_identity_matrix {1.0f}, m_scale {1.0f}, m_pos {0.0f, 0.0f}, m_width {1}, m_height {1}
		{
			deserialize(json);
		}

		Camera2D::Camera2D(const float left, const float right, const float bottom, const float top, const float speed) noexcept
		    : Serializable {this}, m_forward_key {input::Keys::W}, m_back_key {input::Keys::S}, m_left_key {input::Keys::A}, m_right_key {input::Keys::D}, m_moving_fwd {false}, m_moving_back {false}, m_moving_left {false}, m_moving_right {false}, m_speed {1.0f}, m_dirty {true}, m_scaling {1.0f}, m_translation {1.0f}, m_identity_matrix {1.0f}, m_scale {1.0f}, m_pos {0.0f, 0.0f}, m_width {1}, m_height {1}
		{
			create(left, right, bottom, top);
		}

		void Camera2D::create(const float left, const float right, const float bottom, const float top) noexcept
		{
			m_width  = std::max(right, left);
			m_height = std::max(bottom, top);

			m_data.m_projection = glm::ortho(left, right, bottom, top, ORTHO_NEAR, ORTHO_FAR_24BIT);
		}

		void Camera2D::on_event(const events::KeyDown& e) noexcept
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

		void Camera2D::on_event(const events::KeyUp& e) noexcept
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

		void Camera2D::on_event(const events::MouseWheel& e) noexcept
		{
			if (e.m_y_offset < 0)
			{
				m_scale -= 0.1f;
			}
			else
			{
				m_scale += 0.1f;
			}

			zoom(m_scale);
		}

		void Camera2D::on_event(const events::WindowResized& e) noexcept
		{
			create(0.0f, e.m_width, e.m_height, 0.0f);
		}

		void Camera2D::update(const double dt) noexcept
		{
			if (m_moving_fwd)
			{
				move(0.0f, dt * m_speed);
			}

			if (m_moving_back)
			{
				move(0.0f, (dt * m_speed) * -1.0f);
			}

			if (m_moving_left)
			{
				move(dt * m_speed, 0.0f);
			}

			if (m_moving_right)
			{
				move((dt * m_speed) * -1.0f, 0.0f);
			}
		}

		void Camera2D::move(const float x, const float y) noexcept
		{
			m_translation = glm::translate(m_translation, {x, y, 0.0f});
			m_pos.x += x;
			m_pos.y += y;

			m_dirty = true;
		}

		void Camera2D::move_x(const float x) noexcept
		{
			m_translation = glm::translate(m_translation, {x, 0.0f, 0.0f});
			m_pos.x += x;

			m_dirty = true;
		}

		void Camera2D::move_y(const float y) noexcept
		{
			m_translation = glm::translate(m_translation, {0.0f, y, 0.0f});
			m_pos.y += y;

			m_dirty = true;
		}

		void Camera2D::zoom(float scale) noexcept
		{
			m_scale   = std::max(0.1f, scale);
			m_scaling = m_identity_matrix;
			m_scaling = glm::translate(m_scaling, {static_cast<float>(m_width) / 2.0f, static_cast<float>(m_height) / 2.0f, 0.0f});
			m_scaling = glm::scale(m_scaling, {m_scale, m_scale, 1.0f});
			m_scaling = glm::translate(m_scaling, {-(static_cast<float>(m_width) / 2.0f), -(static_cast<float>(m_height) / 2.0f), 0.0f});

			m_dirty = true;
		}

		void Camera2D::set_pos(const float x, const float y) noexcept
		{
			m_translation = glm::translate(m_identity_matrix, {x, y, 0.0f});

			m_pos.x = x;
			m_pos.y = y;

			m_dirty = true;
		}

		void Camera2D::set_pos_x(const float x) noexcept
		{
			m_translation = glm::translate(m_identity_matrix, {x, m_pos.y, 0.0f});

			m_pos.x = x;

			m_dirty = true;
		}

		void Camera2D::set_pos_y(const float y) noexcept
		{
			m_translation = glm::translate(m_identity_matrix, {m_pos.x, y, 0.0f});

			m_pos.y = y;

			m_dirty = true;
		}

		void Camera2D::set_speed(const float speed) noexcept
		{
			m_speed = speed;
		}

		void Camera2D::set_width(const float width) noexcept
		{
			create(0.0f, width, m_height, 0.0f);
		}

		void Camera2D::set_height(const float height) noexcept
		{
			create(0.0f, m_width, height, 0.0f);
		}

		const float Camera2D::get_speed() const noexcept
		{
			return m_speed;
		}

		const int Camera2D::get_width() const noexcept
		{
			return m_width;
		}

		const int Camera2D::get_height() const noexcept
		{
			return m_height;
		}

		const bool Camera2D::is_dirty() const noexcept
		{
			return m_dirty;
		}

		const glm::mat4& Camera2D::get_view() noexcept
		{
			recalculate();
			return m_data.m_model_view;
		}

		const float Camera2D::get_scale() const noexcept
		{
			return m_scale;
		}

		const glm::vec2& Camera2D::get_pos() const noexcept
		{
			return m_pos;
		}

		const glm::mat4& Camera2D::get_proj() noexcept
		{
			return m_data.m_projection;
		}

		Camera2D::Data& Camera2D::get_data() noexcept
		{
			recalculate();
			return m_data;
		}

		nlohmann::json Camera2D::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["x"]           = m_pos.x;
			json["y"]           = m_pos.y;
			json["zoom"]        = m_scale;
			json["speed"]       = m_speed;
			json["width"]       = m_width;
			json["height"]      = m_height;

			return json;
		}

		void Camera2D::deserialize(const nlohmann::json& json)
		{
			m_dirty             = true;
			m_scaling           = glm::mat4 {1.0f};
			m_translation       = glm::mat4 {1.0f};
			m_data.m_model_view = glm::mat4 {1.0f};
			m_moving_fwd        = false;
			m_moving_back       = false;
			m_moving_left       = false;
			m_moving_right      = false;
			m_width             = 0;
			m_height            = 0;
			m_data.m_projection = glm::mat4 {1.0f};

			create(0.0f, json.at("width"), json.at("height"), 0.0f);
			set_pos(json.at("x"), json.at("y"));
			zoom(json.at("zoom"));
			set_speed(json.at("speed"));

			recalculate();
		}

		void Camera2D::recalculate() noexcept
		{
			if (m_dirty)
			{
				m_data.m_model_view = m_translation * m_scaling;
				m_dirty             = false;
			}
		}
	} // namespace graphics
} // namespace galaxy