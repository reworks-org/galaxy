///
/// Camera3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/Config.hpp"

#include "Camera3D.hpp"

namespace galaxy
{
	namespace graphics
	{
		Camera3D::Camera3D()
		    : Serializable {this}, m_dirty_vectors {true}, m_dirty_view {true}, m_pos {0.0f, 0.0f, 0.0f}, m_front {0.0f, 0.0f, -1.0f}, m_up {0.0f, 1.0f, 0.0f}, m_right_axis {0.0f, 0.0f, 0.0f}, m_world_up {0.0f, 0.0f, 0.0f}, m_yaw {-90.0f}, m_pitch {0.0f}, m_speed {2.5f}, m_zoom {45.0f}, m_view {1.0f}, m_forward {input::Keys::W}, m_back {input::Keys::S}, m_left {input::Keys::A}, m_right {input::Keys::D}, m_moving_fwd {false}, m_moving_back {false}, m_moving_left {false}, m_moving_right {false}
		{
			m_sensitivity = SL_HANDLE.config()->get<float>("mouse-sensitivity");
			recalculate();
		}

		Camera3D::Camera3D(const nlohmann::json& json)
		    : Serializable {this}, m_dirty_vectors {true}, m_dirty_view {true}, m_view {1.0f}, m_forward {input::Keys::W}, m_back {input::Keys::S}, m_left {input::Keys::A}, m_right {input::Keys::D}, m_moving_fwd {false}, m_moving_back {false}, m_moving_left {false}, m_moving_right {false}
		{
			m_sensitivity = SL_HANDLE.config()->get<float>("mouse-sensitivity");
			deserialize(json);
		}

		Camera3D::~Camera3D() noexcept
		{
		}

		void Camera3D::on_event(const events::KeyDown& e) noexcept
		{
			if (e.m_keycode == m_forward)
			{
				m_moving_fwd = true;
			}

			if (e.m_keycode == m_back)
			{
				m_moving_back = true;
			}

			if (e.m_keycode == m_left)
			{
				m_moving_left = true;
			}

			if (e.m_keycode == m_right)
			{
				m_moving_right = true;
			}
		}

		void Camera3D::on_event(const events::MouseMoved& e) noexcept
		{
			m_yaw += (e.m_x * m_sensitivity);
			m_pitch += (e.m_y * m_sensitivity);

			if (m_pitch > 89.0f)
			{
				m_pitch = 89.0f;
			}
			else if (m_pitch < -89.0f)
			{
				m_pitch = -89.0f;
			}

			m_dirty_vectors = true;
		}

		void Camera3D::on_event(const events::MouseWheel& e) noexcept
		{
			m_zoom -= e.m_y_offset;

			if (m_zoom > 45.0f)
			{
				m_zoom = 45.0f;
			}
			else if (m_zoom < 1.0f)
			{
				m_zoom = 1.0f;
			}
		}

		void Camera3D::update(const double dt) noexcept
		{
			const float velocity = m_speed * dt;

			if (m_moving_fwd)
			{
				m_pos += m_front * velocity;
				m_moving_fwd = false;

				m_dirty_view = true;
				m_pos.y      = 0.0f;
			}

			if (m_moving_back)
			{
				m_pos -= m_front * velocity;
				m_moving_back = false;

				m_dirty_view = true;
				m_pos.y      = 0.0f;
			}

			if (m_moving_left)
			{
				m_pos -= m_right_axis * velocity;
				m_moving_left = false;

				m_dirty_view = true;
				m_pos.y      = 0.0f;
			}

			if (m_moving_right)
			{
				m_pos += m_right_axis * velocity;
				m_moving_right = false;

				m_dirty_view = true;
				m_pos.y      = 0.0f;
			}
		}

		void Camera3D::reset() noexcept
		{
			m_dirty_vectors = true;
			m_dirty_view    = true;
			m_moving_fwd    = false;
			m_moving_back   = false;
			m_moving_left   = false;
			m_moving_right  = false;

			m_view       = glm::mat4 {1.0f};
			m_pos        = {0.0f, 0.0f, 0.0f};
			m_front      = {0.0f, 0.0f, -1.0f};
			m_up         = {0.0f, 1.0f, 0.0f};
			m_right_axis = {0.0f, 0.0f, 0.0f};
			m_world_up   = {0.0f, 0.0f, 0.0f};

			m_yaw   = -90.0f;
			m_pitch = 0.0f;
			m_speed = 2.5f;
			m_zoom  = 45.0f;

			recalculate();
		}

		const glm::mat4& Camera3D::get_view_matrix() noexcept
		{
			if (m_dirty_view)
			{
				m_view       = glm::lookAt(m_pos, m_pos + m_front, m_up);
				m_dirty_view = false;
			}

			return m_view;
		}

		nlohmann::json Camera3D::serialize()
		{
			nlohmann::json json = "{}"_json;

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

			json["yaw"]   = m_yaw;
			json["pitch"] = m_pitch;
			json["speed"] = m_speed;
			json["zoom"]  = m_zoom;

			return json;
		}

		void Camera3D::deserialize(const nlohmann::json& json)
		{
			reset();

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

			m_yaw   = json.at("yaw");
			m_pitch = json.at("pitch");
			m_speed = json.at("speed");
			m_zoom  = json.at("zoom");

			recalculate();
		}

		void Camera3D::recalculate() noexcept
		{
			if (m_dirty_vectors)
			{
				m_dirty_view    = true;
				m_dirty_vectors = false;

				const auto yaw_radians   = glm::radians(m_yaw);
				const auto pitch_radians = glm::radians(m_pitch);

				m_front.x = glm::cos(yaw_radians) * glm::cos(pitch_radians);
				m_front.y = glm::sin(pitch_radians);
				m_front.z = glm::sin(yaw_radians) * glm::cos(pitch_radians);
				m_front   = glm::normalize(m_front);

				m_right_axis = glm::normalize(glm::cross(m_front, m_world_up));
				m_up         = glm::normalize(glm::cross(m_right_axis, m_front));
			}
		}
	} // namespace graphics
} // namespace galaxy