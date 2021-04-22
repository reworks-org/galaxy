///
/// SpotLight.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/trigonometric.hpp>

#include "SpotLight.hpp"

namespace galaxy
{
	namespace components
	{
		SpotLight::SpotLight() noexcept
		    : Serializable {this}
		{
		}

		SpotLight::SpotLight(const nlohmann::json& json)
		    : Serializable {this}
		{
			deserialize(json);
		}

		SpotLight::SpotLight(SpotLight&& sl) noexcept
		    : Serializable {this}
		{
			this->m_light.m_ambient_intensity  = std::move(sl.m_light.m_ambient_intensity);
			this->m_light.m_diffuse_intensity  = std::move(sl.m_light.m_diffuse_intensity);
			this->m_light.m_specular_intensity = std::move(sl.m_light.m_specular_intensity);
			this->m_light.m_pos                = std::move(sl.m_light.m_pos);
			this->m_light.m_dir                = std::move(sl.m_light.m_dir);
			this->m_light.m_inner_cutoff       = sl.m_light.m_outer_cutoff;
			this->m_light.m_outer_cutoff       = sl.m_light.m_outer_cutoff;
		}

		SpotLight& SpotLight::operator=(SpotLight&& sl) noexcept
		{
			if (this != &sl)
			{
				this->m_light.m_ambient_intensity  = std::move(sl.m_light.m_ambient_intensity);
				this->m_light.m_diffuse_intensity  = std::move(sl.m_light.m_diffuse_intensity);
				this->m_light.m_specular_intensity = std::move(sl.m_light.m_specular_intensity);
				this->m_light.m_pos                = std::move(sl.m_light.m_pos);
				this->m_light.m_dir                = std::move(sl.m_light.m_dir);
				this->m_light.m_inner_cutoff       = sl.m_light.m_outer_cutoff;
				this->m_light.m_outer_cutoff       = sl.m_light.m_outer_cutoff;
			}

			return *this;
		}

		nlohmann::json SpotLight::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["inner-cutoff"] = glm::degrees(glm::acos(m_light.m_inner_cutoff));
			json["outer-cutoff"] = glm::degrees(glm::acos(m_light.m_outer_cutoff));

			json["ambient"]      = nlohmann::json::object();
			json["ambient"]["r"] = m_light.m_ambient_intensity.x;
			json["ambient"]["g"] = m_light.m_ambient_intensity.y;
			json["ambient"]["b"] = m_light.m_ambient_intensity.z;

			json["diffuse"]      = nlohmann::json::object();
			json["diffuse"]["r"] = m_light.m_diffuse_intensity.x;
			json["diffuse"]["g"] = m_light.m_diffuse_intensity.y;
			json["diffuse"]["b"] = m_light.m_diffuse_intensity.z;

			json["specular"]      = nlohmann::json::object();
			json["specular"]["r"] = m_light.m_specular_intensity.x;
			json["specular"]["g"] = m_light.m_specular_intensity.y;
			json["specular"]["b"] = m_light.m_specular_intensity.z;

			json["position"]      = nlohmann::json::object();
			json["position"]["x"] = m_light.m_pos.x;
			json["position"]["y"] = m_light.m_pos.y;
			json["position"]["z"] = m_light.m_pos.z;

			json["direction"]      = nlohmann::json::object();
			json["direction"]["x"] = m_light.m_dir.x;
			json["direction"]["y"] = m_light.m_dir.y;
			json["direction"]["z"] = m_light.m_dir.z;

			return json;
		}

		void SpotLight::deserialize(const nlohmann::json& json)
		{
			m_light.m_inner_cutoff = glm::cos(glm::radians(json.at("inner-cutoff").get<float>()));
			m_light.m_outer_cutoff = glm::cos(glm::radians(json.at("outer-cutoff").get<float>()));

			const auto& ambient           = json.at("ambient");
			m_light.m_ambient_intensity.x = ambient.at("r");
			m_light.m_ambient_intensity.y = ambient.at("g");
			m_light.m_ambient_intensity.z = ambient.at("b");

			const auto& diffuse           = json.at("diffuse");
			m_light.m_diffuse_intensity.x = diffuse.at("r");
			m_light.m_diffuse_intensity.y = diffuse.at("g");
			m_light.m_diffuse_intensity.z = diffuse.at("b");

			const auto& specular           = json.at("specular");
			m_light.m_specular_intensity.x = specular.at("r");
			m_light.m_specular_intensity.y = specular.at("g");
			m_light.m_specular_intensity.z = specular.at("b");

			const auto& position = json.at("position");
			m_light.m_pos.x      = position.at("x");
			m_light.m_pos.y      = position.at("y");
			m_light.m_pos.z      = position.at("z");

			const auto& direction = json.at("direction");
			m_light.m_dir.x       = direction.at("x");
			m_light.m_dir.y       = direction.at("y");
			m_light.m_dir.z       = direction.at("z");
		}
	} // namespace components
} // namespace galaxy