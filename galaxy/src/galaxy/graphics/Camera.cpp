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
			: Serializable {}
			, m_forward_key {input::Keys::W}
			, m_back_key {input::Keys::S}
			, m_left_key {input::Keys::A}
			, m_right_key {input::Keys::D}
			, m_moving_fwd {false}
			, m_moving_back {false}
			, m_moving_left {false}
			, m_moving_right {false}
			, m_speed {1.0f}
			, m_dirty {true}
			, m_scaling {GALAXY_IDENTITY_MATRIX}
			, m_identity_matrix {GALAXY_IDENTITY_MATRIX}
			, m_scale {1.0f}
			, m_pos {0.0f, 0.0f}
			, m_size {1.0f, 1.0f}
		{
			auto& win = core::ServiceLocator<core::Window>::ref();
			create(0.0f, static_cast<float>(win.get_width()), static_cast<float>(win.get_height()), 0.0f);
		}

		Camera::Camera(const nlohmann::json& json) noexcept
			: Serializable {}
			, m_forward_key {input::Keys::W}
			, m_back_key {input::Keys::S}
			, m_left_key {input::Keys::A}
			, m_right_key {input::Keys::D}
			, m_moving_fwd {false}
			, m_moving_back {false}
			, m_moving_left {false}
			, m_moving_right {false}
			, m_speed {1.0f}
			, m_dirty {true}
			, m_scaling {GALAXY_IDENTITY_MATRIX}
			, m_identity_matrix {GALAXY_IDENTITY_MATRIX}
			, m_scale {1.0f}
			, m_pos {0.0f, 0.0f}
			, m_size {1.0f, 1.0f}
		{
			deserialize(json);

			auto& win = core::ServiceLocator<core::Window>::ref();
			create(0.0f, static_cast<float>(win.get_width()), static_cast<float>(win.get_height()), 0.0f);
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
			if (e.m_yoff < 0)
			{
				m_scale -= 0.1f;
			}
			else
			{
				m_scale += 0.1f;
			}

			zoom(m_scale);
		}

		void Camera::on_event(const events::WindowResized& e) noexcept
		{
			create(0.0f, static_cast<float>(e.m_width), static_cast<float>(e.m_height), 0.0f);
		}

		void Camera::update() noexcept
		{
			if (m_moving_fwd)
			{
				move(0.0f, m_speed * GALAXY_DT);
			}

			if (m_moving_back)
			{
				move(0.0f, -(m_speed * GALAXY_DT));
			}

			if (m_moving_left)
			{
				move(m_speed * GALAXY_DT, 0.0f);
			}

			if (m_moving_right)
			{
				move(-(m_speed * GALAXY_DT), 0.0f);
			}
		}

		void Camera::move(const float x, const float y) noexcept
		{
			m_pos.x += x;
			m_pos.y += y;

			m_dirty = true;
		}

		void Camera::zoom(float scale) noexcept
		{
			m_scale = std::clamp(scale, 1.0f, 10.0f);
			m_dirty = true;
		}

		void Camera::set_pos(const float x, const float y) noexcept
		{
			m_pos.x = x;
			m_pos.y = y;

			m_dirty = true;
		}

		void Camera::set_speed(const float speed) noexcept
		{
			m_speed = speed;
		}

		bool Camera::is_dirty() const noexcept
		{
			return m_dirty;
		}

		float Camera::get_speed() const noexcept
		{
			return m_speed;
		}

		float Camera::get_width() const noexcept
		{
			return m_size.x;
		}

		float Camera::get_height() const noexcept
		{
			return m_size.y;
		}

		float Camera::get_scale() const noexcept
		{
			return m_scale;
		}

		const glm::vec2& Camera::get_pos() const noexcept
		{
			return m_pos;
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

		void Camera::create(const float left, const float right, const float bottom, const float top) noexcept
		{
			m_size.x = std::max(right, left);
			m_size.y = std::max(bottom, top);

			m_data.m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		}

		void Camera::recalculate() noexcept
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

		nlohmann::json Camera::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["pos"]["x"]    = m_pos.x;
			json["pos"]["y"]    = m_pos.y;
			json["size"]["x"]   = m_size.x;
			json["size"]["y"]   = m_size.y;
			json["zoom"]        = m_scale;
			json["speed"]       = m_speed;

			return json;
		}

		void Camera::deserialize(const nlohmann::json& json)
		{
			m_moving_fwd   = false;
			m_moving_back  = false;
			m_moving_left  = false;
			m_moving_right = false;
			m_dirty        = true;
			m_scaling      = glm::mat4 {GALAXY_IDENTITY_MATRIX};

			m_data.m_model_view = glm::mat4 {GALAXY_IDENTITY_MATRIX};
			m_data.m_projection = glm::mat4 {GALAXY_IDENTITY_MATRIX};

			const auto& pos = json.at("pos");
			m_pos.x         = pos.at("x");
			m_pos.y         = pos.at("y");

			const auto& size = json.at("size");
			m_size.x         = size.at("x");
			m_size.y         = size.at("y");

			m_scale = std::clamp(json.at("zoom").get<float>(), 1.0f, 10.0f);
			m_speed = json.at("speed");
		}
	} // namespace graphics
} // namespace galaxy