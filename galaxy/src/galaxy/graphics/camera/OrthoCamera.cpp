///
/// OrthoCamera.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "OrthoCamera.hpp"

namespace galaxy
{
	namespace graphics
	{
		OrthoCamera::OrthoCamera() noexcept
		    : Serializable {this}, m_dirty {true}, m_scaling {1.0f}, m_translation {1.0f}, m_model {1.0f}, m_identity_matrix {1.0f}, m_scale {1.0f}, m_pos {0.0f, 0.0f}, m_move_up {false}, m_move_down {false}, m_move_left {false}, m_move_right {false}, m_speed {1.0f}, m_width {1.0f}, m_height {1.0f}, m_projection {1.0f}
		{
		}

		OrthoCamera::OrthoCamera(const nlohmann::json& json) noexcept
		    : Serializable {this}, m_dirty {true}, m_scaling {1.0f}, m_translation {1.0f}, m_model {1.0f}, m_identity_matrix {1.0f}, m_scale {1.0f}, m_pos {0.0f, 0.0f}, m_move_up {false}, m_move_down {false}, m_move_left {false}, m_move_right {false}, m_speed {1.0f}, m_width {1.0f}, m_height {1.0f}, m_projection {1.0f}
		{
			create(0.0f, json.at("width"), json.at("height"), 0.0f);
			m_speed = json.at("speed");
		}

		OrthoCamera::OrthoCamera(const float left, const float right, const float bottom, const float top, const float speed) noexcept
		    : Serializable {this}, m_dirty {true}, m_scaling {1.0f}, m_translation {1.0f}, m_model {1.0f}, m_identity_matrix {1.0f}, m_scale {1.0f}, m_pos {0.0f, 0.0f}, m_move_up {false}, m_move_down {false}, m_move_left {false}, m_move_right {false}, m_speed {speed}, m_width {1.0f}, m_height {1.0f}, m_projection {1.0f}
		{
			create(left, right, bottom, top);
		}

		void OrthoCamera::create(const float left, const float right, const float bottom, const float top) noexcept
		{
			if (right > left)
			{
				m_width = right;
			}
			else
			{
				m_width = left;
			}

			if (bottom > top)
			{
				m_height = bottom;
			}
			else
			{
				m_height = top;
			}

			m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		}

		void OrthoCamera::on_event(const events::KeyDown& e) noexcept
		{
			switch (e.m_keycode)
			{
				case input::Keys::W:
					m_move_up = true;
					break;

				case input::Keys::S:
					m_move_down = true;
					break;

				case input::Keys::A:
					m_move_left = true;
					break;

				case input::Keys::D:
					m_move_right = true;
					break;
			}
		}

		void OrthoCamera::on_event(const events::KeyUp& e) noexcept
		{
			switch (e.m_keycode)
			{
				case input::Keys::W:
					m_move_up = false;
					break;

				case input::Keys::S:
					m_move_down = false;
					break;

				case input::Keys::A:
					m_move_left = false;
					break;

				case input::Keys::D:
					m_move_right = false;
					break;
			}
		}

		void OrthoCamera::on_event(const events::MouseWheel& e) noexcept
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

		void OrthoCamera::on_event(const events::WindowResized& e) noexcept
		{
			create(0.0f, e.m_width, e.m_height, 0.0f);
		}

		void OrthoCamera::update(const double ts) noexcept
		{
			if (m_move_up)
			{
				move(0.0f, ts * m_speed);
			}

			if (m_move_down)
			{
				move(0.0f, (ts * m_speed) * -1.0f);
			}

			if (m_move_left)
			{
				move(ts * m_speed, 0.0f);
			}

			if (m_move_right)
			{
				move((ts * m_speed) * -1.0f, 0.0f);
			}
		}

		void OrthoCamera::move(const float x, const float y) noexcept
		{
			m_translation = glm::translate(m_translation, {x, y, 0.0f});
			m_pos.x += x;
			m_pos.y += y;

			m_dirty = true;
		}

		void OrthoCamera::move_x(const float x) noexcept
		{
			m_translation = glm::translate(m_translation, {x, 0.0f, 0.0f});
			m_pos.x += x;

			m_dirty = true;
		}

		void OrthoCamera::move_y(const float y) noexcept
		{
			m_translation = glm::translate(m_translation, {0.0f, y, 0.0f});
			m_pos.y += y;

			m_dirty = true;
		}

		void OrthoCamera::zoom(float scale) noexcept
		{
			if (scale < 0.2f)
			{
				scale = 0.2f;
			}

			m_scale   = scale;
			m_scaling = m_identity_matrix;
			m_scaling = glm::translate(m_scaling, {m_width / 2.0f, m_height / 2.0f, 0.0f});
			m_scaling = glm::scale(m_scaling, {m_scale, m_scale, 1.0f});
			m_scaling = glm::translate(m_scaling, {-(m_width / 2.0f), -(m_height / 2.0f), 0.0f});

			m_dirty = true;
		}

		void OrthoCamera::set_pos(const float x, const float y) noexcept
		{
			m_translation = glm::translate(m_identity_matrix, {x, y, 0.0f});

			m_pos.x = x;
			m_pos.y = y;

			m_dirty = true;
		}

		void OrthoCamera::set_speed(const float speed) noexcept
		{
			m_speed = speed;
		}

		void OrthoCamera::set_width(const float width) noexcept
		{
			create(0.0f, width, m_height, 0.0f);
		}

		void OrthoCamera::set_height(const float height) noexcept
		{
			create(0.0f, m_width, height, 0.0f);
		}

		const float OrthoCamera::get_speed() const noexcept
		{
			return m_speed;
		}

		const float OrthoCamera::get_width() const noexcept
		{
			return m_width;
		}

		const float OrthoCamera::get_height() const noexcept
		{
			return m_height;
		}

		const bool OrthoCamera::is_dirty() const noexcept
		{
			return m_dirty;
		}

		const glm::mat4& OrthoCamera::get_view() noexcept
		{
			recalculate();
			return m_model;
		}

		const float OrthoCamera::get_scale() const noexcept
		{
			return m_scale;
		}

		const glm::vec2& OrthoCamera::get_pos() const noexcept
		{
			return m_pos;
		}

		const glm::mat4& OrthoCamera::get_proj() noexcept
		{
			return m_projection;
		}

		nlohmann::json OrthoCamera::serialize()
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

		void OrthoCamera::deserialize(const nlohmann::json& json)
		{
			m_dirty       = true;
			m_scaling     = glm::mat4 {1.0f};
			m_translation = glm::mat4 {1.0f};
			m_model       = glm::mat4 {1.0f};
			m_move_up     = false;
			m_move_down   = false;
			m_move_left   = false;
			m_move_right  = false;
			m_width       = 0.0f;
			m_height      = 0.0f;
			m_projection  = glm::mat4 {1.0f};

			create(0.0f, json.at("width"), json.at("height"), 0.0f);
			set_pos(json.at("x"), json.at("y"));
			zoom(json.at("zoom"));
			set_speed(json.at("speed"));

			recalculate();
		}

		void OrthoCamera::recalculate() noexcept
		{
			if (m_dirty)
			{
				m_model = m_translation * m_scaling;
				m_dirty = false;
			}
		}
	} // namespace graphics
} // namespace galaxy