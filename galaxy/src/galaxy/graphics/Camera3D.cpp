///
/// Camera3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/fs/Config.hpp"

#include "Camera3D.hpp"

namespace galaxy
{
	namespace graphics
	{
		Camera3D::Camera3D()
		    : Serializable {this}
		{
			m_sensitivity  = SL_HANDLE.config()->get<float>("mouse-sensitivity");
			m_aspect_ratio = SL_HANDLE.window()->get_width() / SL_HANDLE.window()->get_height();

			m_pos       = {0.0f, 0.0f, 0.0f};
			m_delta     = {0.0f, 0.0f, 0.0f};
			m_focal     = {0.0f, 0.0f, 0.0f};
			m_dir       = {0.0f, 0.0f, 0.0f};
			m_up        = {0.0f, 1.0f, 0.0f};
			m_mouse_pos = {0.0f, 0.0f, 0.0f};

			m_moving_fwd   = false;
			m_moving_back  = false;
			m_moving_left  = false;
			m_moving_right = false;

			m_view = glm::mat4 {1.0f};
			m_proj = glm::mat4 {1.0f};
		}

		Camera3D::Camera3D(const nlohmann::json& json)
		    : Serializable {this}
		{
			m_sensitivity  = SL_HANDLE.config()->get<float>("mouse-sensitivity");
			m_aspect_ratio = SL_HANDLE.window()->get_width() / SL_HANDLE.window()->get_height();

			m_pos       = {0.0f, 0.0f, 0.0f};
			m_delta     = {0.0f, 0.0f, 0.0f};
			m_focal     = {0.0f, 0.0f, 0.0f};
			m_dir       = {0.0f, 0.0f, 0.0f};
			m_up        = {0.0f, 1.0f, 0.0f};
			m_mouse_pos = {0.0f, 0.0f, 0.0f};

			m_moving_fwd   = false;
			m_moving_back  = false;
			m_moving_left  = false;
			m_moving_right = false;

			m_view = glm::mat4 {1.0f};
			m_proj = glm::mat4 {1.0f};

			deserialize(json);
		}

		void Camera3D::set_mode(const Mode mode) noexcept
		{
			m_mode = mode;
		}

		void Camera3D::set_position(const glm::vec3& pos) noexcept
		{
			m_pos.x = pos.x;
			m_pos.y = pos.y;
			m_pos.z = pos.z;
		}

		void Camera3D::set_focal(const glm::vec3& look_at) noexcept
		{
			m_focal.x = look_at.x;
			m_focal.y = look_at.y;
			m_focal.z = look_at.z;
		}

		void Camera3D::set_fov(const float fov) noexcept
		{
			m_fov = fov;
		}

		void Camera3D::set_near(const float near_clip) noexcept
		{
			m_near = near_clip;
		}

		void Camera3D::set_far(const float far_clip) noexcept
		{
			m_far = far_clip;
		}

		void Camera3D::set_speed(const float speed) noexcept
		{
			m_speed = speed;
		}

		void Camera3D::pitch(float degrees) noexcept
		{
			if (degrees < -m_max_pitch_rate)
			{
				degrees = -m_max_pitch_rate;
			}
			else if (degrees > m_max_pitch_rate)
			{
				degrees = m_max_pitch_rate;
			}

			m_pitch += degrees;

			if (m_pitch > 360.0f)
			{
				m_pitch -= 360.0f;
			}
			else if (m_pitch < -360.0f)
			{
				m_pitch += 360.0f;
			}
		}

		void Camera3D::heading(float degrees) noexcept
		{
			if (degrees < -m_max_heading_rate)
			{
				degrees = -m_max_heading_rate;
			}
			else if (degrees > m_max_heading_rate)
			{
				degrees = m_max_heading_rate;
			}

			if (m_pitch > 90 && m_pitch < 270 || (m_pitch < -90 && m_pitch > -270))
			{
				m_heading -= degrees;
			}
			else
			{
				m_heading += degrees;
			}

			if (m_heading > 360.0f)
			{
				m_heading -= 360.0f;
			}
			else if (m_heading < -360.0f)
			{
				m_heading += 360.0f;
			}
		}

		void Camera3D::on_event(const events::KeyDown& e) noexcept
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

			if (m_mode == Mode::FREE)
			{
				// Different so it staggers rather than repeats.
				// This is to allow easier movement of up and down axis.
				if (e.m_keycode == m_up_key)
				{
					m_delta += (m_up * m_speed);
				}

				if (e.m_keycode == m_down_key)
				{
					m_delta -= (m_up * m_speed);
				}
			}
		}

		void Camera3D::on_event(const events::KeyUp& e) noexcept
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

		void Camera3D::on_event(const events::MouseMoved& e) noexcept
		{
			const glm::vec3 new_pos = {e.m_x, e.m_y, 0.0f};
			const glm::vec3 delta   = m_mouse_pos - new_pos;

			heading((delta.x * m_sensitivity) * (m_speed / 100.0f));
			pitch((delta.y * m_sensitivity) * (m_speed / 100.0f));

			m_mouse_pos = new_pos;
		}

		void Camera3D::on_event(const events::MouseWheel& e) noexcept
		{
			if (m_mode == Mode::FREE)
			{
				m_fov -= e.m_y_offset;
				if (m_fov > 110.0f)
				{
					m_fov = 110.0f;
				}
				else if (m_fov < 45.0f)
				{
					m_fov = 45.0f;
				}
			}
		}

		void Camera3D::on_event(const events::WindowResized& e) noexcept
		{
			m_aspect_ratio = e.m_width / e.m_height;
		}

		void Camera3D::update(const double dt) noexcept
		{
			const auto velocity = m_speed * static_cast<float>(dt);

			if (m_moving_fwd)
			{
				m_delta += (m_dir * velocity);
			}

			if (m_moving_back)
			{
				m_delta -= (m_dir * velocity);
			}

			if (m_moving_left)
			{
				m_delta -= (glm::cross(m_dir, m_up) * velocity);
			}

			if (m_moving_right)
			{
				m_delta += (glm::cross(m_dir, m_up) * velocity);
			}

			m_dir = glm::normalize(m_focal - m_pos);

			if (m_mode == Mode::FREE)
			{
				m_proj = glm::perspective(m_fov, m_aspect_ratio, m_near, m_far);

				glm::vec3 axis         = glm::cross(m_dir, m_up);
				glm::quat pitch_quat   = glm::angleAxis(m_pitch, axis);
				glm::quat heading_quat = glm::angleAxis(m_heading, m_up);
				glm::quat temp         = glm::cross(pitch_quat, heading_quat);
				temp                   = glm::normalize(temp);

				m_dir = glm::rotate(temp, m_dir);
				m_pos += m_delta;
				m_focal = m_pos + m_dir * 1.0f;
				m_heading *= .5;
				m_pitch *= .5;
				m_delta = m_delta * .8f;
			}

			m_view = glm::lookAt(m_pos, m_focal, m_up);
		}

		void Camera3D::reset() noexcept
		{
			float m_fov  = 45.0f;
			float m_near = 0.1f;
			float m_far  = 100.0f;

			float m_heading          = 0.0f;
			float m_pitch            = 0.0f;
			float m_max_pitch_rate   = 5.0f;
			float m_max_heading_rate = 5.0f;

			m_pos       = {0.0f, 0.0f, 0.0f};
			m_delta     = {0.0f, 0.0f, 0.0f};
			m_focal     = {0.0f, 0.0f, 0.0f};
			m_dir       = {0.0f, 0.0f, 0.0f};
			m_up        = {0.0f, 1.0f, 0.0f};
			m_mouse_pos = {0.0f, 0.0f, 0.0f};

			m_moving_fwd   = false;
			m_moving_back  = false;
			m_moving_left  = false;
			m_moving_right = false;

			m_view = glm::mat4 {1.0f};
			m_proj = glm::mat4 {1.0f};

			float m_speed = 1.0f;
		}

		const glm::mat4& Camera3D::get_view() const noexcept
		{
			return m_view;
		}

		const glm::mat4& Camera3D::get_proj() const noexcept
		{
			return m_proj;
		}

		const glm::vec3& Camera3D::get_pos() const noexcept
		{
			return m_pos;
		}

		const glm::vec3& Camera3D::get_focus() const noexcept
		{
			return m_focal;
		}

		const glm::vec3& Camera3D::get_dir() const noexcept
		{
			return m_dir;
		}

		nlohmann::json Camera3D::serialize()
		{
			nlohmann::json json = "{}"_json;

			/*
			json["pos"]["x"] = m_pos.x;
			json["pos"]["y"] = m_pos.y;
			json["pos"]["z"] = m_pos.z;

			json["front"]["x"] = m_front.x;
			json["front"]["y"] = m_front.y;
			json["front"]["z"] = m_front.z;

			json["up"]["x"] = m_up.x;
			json["up"]["y"] = m_up.y;
			json["up"]["z"] = m_up.z;

			json["right-axis"]["x"] = m_right_axis.x;
			json["right-axis"]["y"] = m_right_axis.y;
			json["right-axis"]["z"] = m_right_axis.z;

			json["world-up"]["x"] = m_world_up.x;
			json["world-up"]["y"] = m_world_up.y;
			json["world-up"]["z"] = m_world_up.z;

			json["yaw"]     = m_yaw;
			json["pitch"]   = m_pitch;
			json["fov"]     = m_fov;
			json["prev-mx"] = m_prev_mx;
			json["prev-my"] = m_prev_my;
			json["speed"]   = m_speed;
			*/

			return json;
		}

		void Camera3D::deserialize(const nlohmann::json& json)
		{
			reset();

			/*
			const auto& pos = json.at("pos");
			m_pos.x         = pos.at("x");
			m_pos.y         = pos.at("y");
			m_pos.z         = pos.at("z");

			const auto& front = json.at("front");
			m_front.x         = front.at("x");
			m_front.y         = front.at("y");
			m_front.z         = front.at("z");

			const auto& up = json.at("up");
			m_up.x         = up.at("x");
			m_up.y         = up.at("y");
			m_up.z         = up.at("z");

			const auto& right_axis = json.at("right-axis");
			m_right_axis.x         = right_axis.at("x");
			m_right_axis.y         = right_axis.at("y");
			m_right_axis.z         = right_axis.at("z");

			const auto& world_up = json.at("world-up");
			m_world_up.x         = world_up.at("x");
			m_world_up.y         = world_up.at("y");
			m_world_up.z         = world_up.at("z");

			m_yaw     = json.at("yaw");
			m_pitch   = json.at("pitch");
			m_fov     = json.at("fov");
			m_prev_mx = json.at("prev-mx");
			m_prev_my = json.at("prev-my");
			m_speed   = json.at("speed");
			*/
		}
	} // namespace graphics
} // namespace galaxy