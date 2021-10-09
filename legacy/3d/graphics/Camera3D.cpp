///
/// Camera3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <magic_enum.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/fs/Config.hpp"

#include "Camera3D.hpp"

namespace galaxy
{
	namespace graphics
	{
		Camera3D::Camera3D()
		    : Camera {}, Serializable {this}
		{
			m_sensitivity  = SL_HANDLE.config()->get<float>("mouse-sensitivity");
			m_aspect_ratio = static_cast<float>(static_cast<double>(SL_HANDLE.window()->get_width()) / static_cast<double>(SL_HANDLE.window()->get_height()));
			m_fov          = SL_HANDLE.config()->get<float>("camera-fov");
			m_near         = SL_HANDLE.config()->get<float>("camera-near");
			m_far          = SL_HANDLE.config()->get<float>("camera-far");

			reset();
		}

		Camera3D::Camera3D(const nlohmann::json& json)
		    : Camera {}, Serializable {this}
		{
			m_sensitivity  = SL_HANDLE.config()->get<float>("mouse-sensitivity");
			m_aspect_ratio = static_cast<float>(SL_HANDLE.window()->get_width()) / static_cast<float>(SL_HANDLE.window()->get_height());
			m_fov          = SL_HANDLE.config()->get<float>("camera-fov");
			m_near         = SL_HANDLE.config()->get<float>("camera-near");
			m_far          = SL_HANDLE.config()->get<float>("camera-far");

			deserialize(json);
		}

		void Camera3D::set_mode(const Mode mode) noexcept
		{
			m_mode = mode;
		}

		void Camera3D::set_position(const glm::vec3& pos) noexcept
		{
			m_data.m_pos.x = pos.x;
			m_data.m_pos.y = pos.y;
			m_data.m_pos.z = pos.z;
		}

		void Camera3D::move_position(const glm::vec3& move) noexcept
		{
			m_data.m_pos.x += move.x;
			m_data.m_pos.y += move.y;
			m_data.m_pos.z += move.z;
		}

		void Camera3D::set_focal(const glm::vec3& look_at) noexcept
		{
			m_focal.x = look_at.x;
			m_focal.y = look_at.y;
			m_focal.z = look_at.z;
		}

		void Camera3D::set_speed(const float speed) noexcept
		{
			m_speed = speed;
		}

		void Camera3D::pitch(float degrees) noexcept
		{
			constexpr const auto max_pitch_rate = 5.0f;

			if (degrees < -max_pitch_rate)
			{
				degrees = -max_pitch_rate;
			}
			else if (degrees > max_pitch_rate)
			{
				degrees = max_pitch_rate;
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
			constexpr const auto max_heading_rate = 5.0f;

			if (degrees < -max_heading_rate)
			{
				degrees = -max_heading_rate;
			}
			else if (degrees > max_heading_rate)
			{
				degrees = max_heading_rate;
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
				if (e.m_keycode == m_up_key)
				{
					m_moving_up = true;
				}

				if (e.m_keycode == m_down_key)
				{
					m_moving_down = true;
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

			if (m_mode == Mode::FREE)
			{
				if (e.m_keycode == m_up_key)
				{
					m_moving_up = false;
				}

				if (e.m_keycode == m_down_key)
				{
					m_moving_down = false;
				}
			}
		}

		void Camera3D::on_event(const events::MouseMoved& e) noexcept
		{
			const glm::vec3 new_pos = {e.m_x, e.m_y, 0.0f};
			const glm::vec3 delta   = m_mouse_pos - new_pos;

			heading(delta.x * (m_sensitivity / 1000.0f));
			pitch(delta.y * (m_sensitivity / 1000.0f));

			m_mouse_pos = new_pos;
		}

		void Camera3D::on_event(const events::MouseWheel& e) noexcept
		{
			if (m_mode == Mode::FREE)
			{
				constexpr const auto min_fov = 45.0f;
				constexpr const auto max_fov = 110.0f;

				m_fov -= e.m_y_offset;

				if (m_fov < min_fov)
				{
					m_fov = min_fov;
				}
				else if (m_fov > max_fov)
				{
					m_fov = max_fov;
				}
			}
		}

		void Camera3D::on_event(const events::WindowResized& e) noexcept
		{
			auto w = e.m_width;
			auto h = e.m_height;

			if (w <= 0)
			{
				w = 1;
			}

			if (h <= 0)
			{
				h = 1;
			}

			m_aspect_ratio = static_cast<float>(w) / static_cast<float>(h);
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

			m_dir         = glm::normalize(m_focal - m_data.m_pos);
			m_data.m_proj = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near, m_far);

			if (m_mode == Mode::FREE)
			{
				if (m_moving_up)
				{
					m_delta += (m_up * velocity);
				}

				if (m_moving_down)
				{
					m_delta -= (m_up * velocity);
				}

				glm::vec3 axis         = glm::cross(m_dir, m_up);
				glm::quat pitch_quat   = glm::angleAxis(m_pitch, axis);
				glm::quat heading_quat = glm::angleAxis(m_heading, m_up);
				glm::quat temp         = glm::cross(pitch_quat, heading_quat);
				temp                   = glm::normalize(temp);

				m_dir = glm::rotate(temp, m_dir);
				m_data.m_pos += m_delta;
				m_focal = m_data.m_pos + m_dir * 1.0f;
				m_heading *= .5;
				m_pitch *= .5;
				m_delta = m_delta * .8f;
			}

			m_data.m_view = glm::lookAt(m_data.m_pos, m_focal, m_up);
		}

		void Camera3D::reset() noexcept
		{
			m_mode = graphics::Camera3D::Mode::FREE;

			m_heading = 0.0f;
			m_pitch   = 0.0f;

			m_data.m_pos = {0.0f, 0.0f, 3.0f};
			m_delta      = {0.0f, 0.0f, 0.0f};
			m_focal      = {0.0f, 0.0f, 0.0f};
			m_dir        = {0.0f, 0.0f, 0.0f};
			m_up         = {0.0f, 1.0f, 0.0f};
			m_mouse_pos  = {0.0f, 0.0f, 0.0f};

			m_moving_fwd   = false;
			m_moving_back  = false;
			m_moving_left  = false;
			m_moving_right = false;
			m_moving_up    = false;
			m_moving_down  = false;

			m_data.m_view = glm::mat4 {1.0f};
			m_data.m_proj = glm::mat4 {1.0f};

			m_speed = 0.5f;
		}

		const glm::mat4& Camera3D::get_view() noexcept
		{
			return m_data.m_view;
		}

		const glm::mat4& Camera3D::get_proj() noexcept
		{
			return m_data.m_proj;
		}

		const Camera3D::Data& Camera3D::get_data() const noexcept
		{
			return m_data;
		}

		nlohmann::json Camera3D::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["mode"]    = magic_enum::enum_name<Camera3D::Mode>(m_mode);
			json["heading"] = m_heading;
			json["pitch"]   = m_pitch;
			json["speed"]   = m_speed;

			json["pos"]      = nlohmann::json::object();
			json["pos"]["x"] = m_data.m_pos.x;
			json["pos"]["y"] = m_data.m_pos.y;
			json["pos"]["z"] = m_data.m_pos.z;

			json["delta"]      = nlohmann::json::object();
			json["delta"]["x"] = m_delta.x;
			json["delta"]["y"] = m_delta.y;
			json["delta"]["z"] = m_delta.z;

			json["focal"]      = nlohmann::json::object();
			json["focal"]["x"] = m_focal.x;
			json["focal"]["y"] = m_focal.y;
			json["focal"]["z"] = m_focal.z;

			json["dir"]      = nlohmann::json::object();
			json["dir"]["x"] = m_dir.x;
			json["dir"]["y"] = m_dir.y;
			json["dir"]["z"] = m_dir.z;

			json["up"]      = nlohmann::json::object();
			json["up"]["x"] = m_up.x;
			json["up"]["y"] = m_up.y;
			json["up"]["z"] = m_up.z;

			json["mouse-pos"]      = nlohmann::json::object();
			json["mouse-pos"]["x"] = m_mouse_pos.x;
			json["mouse-pos"]["y"] = m_mouse_pos.y;
			json["mouse-pos"]["z"] = m_mouse_pos.z;

			return json;
		}

		void Camera3D::deserialize(const nlohmann::json& json)
		{
			reset();

			m_mode    = magic_enum::enum_cast<Camera3D::Mode>(json.at("mode").get<std::string>()).value();
			m_heading = json.at("heading");
			m_pitch   = json.at("pitch");
			m_speed   = json.at("speed");

			const auto& pos = json.at("pos");
			m_data.m_pos.x  = pos.at("x");
			m_data.m_pos.y  = pos.at("y");
			m_data.m_pos.z  = pos.at("z");

			const auto& delta = json.at("delta");
			m_delta.x         = delta.at("x");
			m_delta.y         = delta.at("y");
			m_delta.z         = delta.at("z");

			const auto& focal = json.at("focal");
			m_focal.x         = focal.at("x");
			m_focal.y         = focal.at("y");
			m_focal.z         = focal.at("z");

			const auto& dir = json.at("dir");
			m_dir.x         = dir.at("x");
			m_dir.y         = dir.at("y");
			m_dir.z         = dir.at("z");

			const auto& up = json.at("up");
			m_up.x         = up.at("x");
			m_up.y         = up.at("y");
			m_up.z         = up.at("z");

			const auto& mouse_pos = json.at("mouse-pos");
			m_mouse_pos.x         = mouse_pos.at("x");
			m_mouse_pos.y         = mouse_pos.at("y");
			m_mouse_pos.z         = mouse_pos.at("z");
		}
	} // namespace graphics
} // namespace galaxy