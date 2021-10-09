///
/// DirectionLight.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/matrix_transform.hpp>

#include "Direction.hpp"

namespace galaxy
{
	namespace graphics
	{
		DirectionLight::DirectionLight() noexcept
		    : Serializable {this}, m_dirty {true}, m_target {0.0f, 0.0f, 0.0f}, m_fov {60.0f}
		{
			m_data.m_ambient_intensity.x = 0.05f;
			m_data.m_ambient_intensity.y = 0.05f;
			m_data.m_ambient_intensity.z = 0.05f;

			m_data.m_diffuse_intensity.x = 0.4f;
			m_data.m_diffuse_intensity.y = 0.4f;
			m_data.m_diffuse_intensity.z = 0.4f;

			m_data.m_specular_intensity.x = 0.5f;
			m_data.m_specular_intensity.y = 0.5f;
			m_data.m_specular_intensity.z = 0.5f;

			m_data.m_dir.x = 0.0f;
			m_data.m_dir.y = 0.0f;
			m_data.m_dir.z = 0.0f;

			m_data.m_view = glm::mat4 {1.0f};
			m_data.m_proj = glm::mat4 {1.0f};

			m_data.m_near = 0.1f;
			m_data.m_far  = 16.0f;
		}

		DirectionLight::DirectionLight(const nlohmann::json& json)
		    : Serializable {this}, m_dirty {true}
		{
			deserialize(json);
		}

		void DirectionLight::set_ambient_intensity(const float x, const float y, const float z) noexcept
		{
			m_dirty                    = true;
			m_data.m_ambient_intensity = {x, y, z};
		}

		void DirectionLight::set_diffuse_intensity(const float x, const float y, const float z) noexcept
		{
			m_dirty                    = true;
			m_data.m_diffuse_intensity = {x, y, z};
		}

		void DirectionLight::set_specular_intensity(const float x, const float y, const float z) noexcept
		{
			m_dirty                     = true;
			m_data.m_specular_intensity = {x, y, z};
		}

		void DirectionLight::set_dir(const float x, const float y, const float z) noexcept
		{
			m_dirty      = true;
			m_data.m_dir = {x, y, z};
			m_data.m_dir = -m_data.m_dir;
		}

		void DirectionLight::set_near(const float near) noexcept
		{
			m_dirty       = true;
			m_data.m_near = near;
		}

		void DirectionLight::set_far(const float far) noexcept
		{
			m_dirty      = true;
			m_data.m_far = far;
		}

		void DirectionLight::set_target(const float x, const float y, const float z) noexcept
		{
			m_dirty  = true;
			m_target = {x, y, z};
		}

		void DirectionLight::set_fov(const float fov) noexcept
		{
			m_dirty = true;
			m_fov   = std::clamp(fov, 0.0f, 360.0f);
		}

		const glm::vec3& DirectionLight::get_target() const noexcept
		{
			return m_target;
		}

		const float DirectionLight::get_fov() const noexcept
		{
			return m_fov;
		}

		const light::Directional& DirectionLight::get_data() noexcept
		{
			if (m_dirty)
			{
				m_dirty = false;

				const auto pos = m_target - m_data.m_dir * 200.0f;
				m_data.m_view  = glm::lookAt(pos, m_target, {0.0f, 1.0f, 0.0f});
				m_data.m_proj  = glm::perspective(m_fov, 1.0f, m_data.m_near, m_data.m_far);
			}
			return m_data;
		}

		const bool DirectionLight::is_dirty() const noexcept
		{
			return m_dirty;
		}

		nlohmann::json DirectionLight::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["ambient"]      = nlohmann::json::object();
			json["ambient"]["r"] = m_data.m_ambient_intensity.x;
			json["ambient"]["g"] = m_data.m_ambient_intensity.y;
			json["ambient"]["b"] = m_data.m_ambient_intensity.z;

			json["diffuse"]      = nlohmann::json::object();
			json["diffuse"]["r"] = m_data.m_diffuse_intensity.x;
			json["diffuse"]["g"] = m_data.m_diffuse_intensity.y;
			json["diffuse"]["b"] = m_data.m_diffuse_intensity.z;

			json["specular"]      = nlohmann::json::object();
			json["specular"]["r"] = m_data.m_specular_intensity.x;
			json["specular"]["g"] = m_data.m_specular_intensity.y;
			json["specular"]["b"] = m_data.m_specular_intensity.z;

			json["direction"]      = nlohmann::json::object();
			json["direction"]["x"] = m_data.m_dir.x;
			json["direction"]["y"] = m_data.m_dir.y;
			json["direction"]["z"] = m_data.m_dir.z;

			json["target"]      = nlohmann::json::object();
			json["target"]["x"] = m_target.x;
			json["target"]["y"] = m_target.y;
			json["target"]["z"] = m_target.z;

			json["near"] = m_data.m_near;
			json["far"]  = m_data.m_far;
			json["fov"]  = m_fov;

			return json;
		}

		void DirectionLight::deserialize(const nlohmann::json& json)
		{
			const auto& ambient          = json.at("ambient");
			m_data.m_ambient_intensity.x = ambient.at("r");
			m_data.m_ambient_intensity.y = ambient.at("g");
			m_data.m_ambient_intensity.z = ambient.at("b");

			const auto& diffuse          = json.at("diffuse");
			m_data.m_diffuse_intensity.x = diffuse.at("r");
			m_data.m_diffuse_intensity.y = diffuse.at("g");
			m_data.m_diffuse_intensity.z = diffuse.at("b");

			const auto& specular          = json.at("specular");
			m_data.m_specular_intensity.x = specular.at("r");
			m_data.m_specular_intensity.y = specular.at("g");
			m_data.m_specular_intensity.z = specular.at("b");

			const auto& direction = json.at("direction");
			m_data.m_dir.x        = direction.at("x");
			m_data.m_dir.y        = direction.at("y");
			m_data.m_dir.z        = direction.at("z");

			const auto& target = json.at("target");
			m_target.x         = target.at("x");
			m_target.y         = target.at("y");
			m_target.z         = target.at("z");

			m_data.m_near = json.at("near");
			m_data.m_far  = json.at("far");
			m_fov         = json.at("fov");
		}
	} // namespace graphics
} // namespace galaxy