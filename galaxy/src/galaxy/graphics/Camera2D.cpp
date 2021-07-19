///
/// Camera2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"

#include "Camera2D.hpp"

namespace galaxy
{
	namespace graphics
	{
		Camera2D::Camera2D() noexcept
		    : Serializable {this}, m_forward_key {input::Keys::W}, m_back_key {input::Keys::S}, m_left_key {input::Keys::A}, m_right_key {input::Keys::D}, m_moving_fwd {false}, m_moving_back {false}, m_moving_left {false}, m_moving_right {false}, m_speed {1.0f}, m_dirty {true}, m_scaling {1.0f}, m_identity_matrix {1.0f}, m_scale {1.0f}, m_pos {0.0f, 0.0f}, m_size {1.0f, 1.0f}
		{
			create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);
		}

		Camera2D::Camera2D(const nlohmann::json& json) noexcept
		    : Serializable {this}, m_forward_key {input::Keys::W}, m_back_key {input::Keys::S}, m_left_key {input::Keys::A}, m_right_key {input::Keys::D}, m_moving_fwd {false}, m_moving_back {false}, m_moving_left {false}, m_moving_right {false}, m_speed {1.0f}, m_dirty {true}, m_scaling {1.0f}, m_identity_matrix {1.0f}, m_scale {1.0f}, m_pos {0.0f, 0.0f}, m_size {1.0f, 1.0f}
		{
			deserialize(json);
			create(0.0f, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(), 0.0f);
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
				move(0.0f, -(dt * m_speed));
			}

			if (m_moving_left)
			{
				move(dt * m_speed, 0.0f);
			}

			if (m_moving_right)
			{
				move(-(dt * m_speed), 0.0f);
			}
		}

		void Camera2D::move(const float x, const float y) noexcept
		{
			m_pos.x += x;
			m_pos.y += y;

			m_dirty = true;
		}

		void Camera2D::zoom(float scale) noexcept
		{
			m_scale = std::clamp(scale, 1.0f, 10.0f);

			m_dirty = true;
		}

		void Camera2D::set_pos(const float x, const float y) noexcept
		{
			m_pos.x = x;
			m_pos.y = y;

			m_dirty = true;
		}

		void Camera2D::set_speed(const float speed) noexcept
		{
			m_speed = speed;
		}

		const float Camera2D::get_speed() const noexcept
		{
			return m_speed;
		}

		const float Camera2D::get_width() const noexcept
		{
			return m_size.x;
		}

		const float Camera2D::get_height() const noexcept
		{
			return m_size.y;
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

		math::AABB& Camera2D::get_aabb() noexcept
		{
			m_aabb = {m_pos, m_pos + m_size};
			m_aabb.update_area();

			return m_aabb;
		}

		void Camera2D::create(const float left, const float right, const float bottom, const float top) noexcept
		{
			m_size.x = std::max(right, left);
			m_size.y = std::max(bottom, top);

			m_data.m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		}

		void Camera2D::recalculate() noexcept
		{
			if (m_dirty)
			{
				m_scaling = m_identity_matrix;
				m_scaling = glm::translate(m_scaling, {m_size / 2.0f, 0.0f});
				m_scaling = glm::scale(m_scaling, {m_scale, m_scale, 1.0f});
				m_scaling = glm::translate(m_scaling, {-m_size / 2.0f, 0.0f});

				m_data.m_model_view = glm::translate(m_identity_matrix, {m_pos, 0.0f}) * m_scaling;
				m_dirty             = false;
			}
		}

		nlohmann::json Camera2D::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["x"]           = m_pos.x;
			json["y"]           = m_pos.y;
			json["zoom"]        = m_scale;
			json["speed"]       = m_speed;

			return json;
		}

		void Camera2D::deserialize(const nlohmann::json& json)
		{
			m_dirty             = true;
			m_scaling           = glm::mat4 {1.0f};
			m_data.m_model_view = glm::mat4 {1.0f};
			m_moving_fwd        = false;
			m_moving_back       = false;
			m_moving_left       = false;
			m_moving_right      = false;
			m_data.m_projection = glm::mat4 {1.0f};

			set_pos(json.at("x"), json.at("y"));
			zoom(json.at("zoom"));
			set_speed(json.at("speed"));
		}
	} // namespace graphics
} // namespace galaxy