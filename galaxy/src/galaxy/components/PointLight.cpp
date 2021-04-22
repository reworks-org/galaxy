///
/// PointLight.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "PointLight.hpp"

namespace galaxy
{
	namespace components
	{
		PointLight::PointLight() noexcept
		    : Serializable {this}
		{
		}

		PointLight::PointLight(const nlohmann::json& json)
		    : Serializable {this}
		{
			deserialize(json);
		}

		PointLight::PointLight(PointLight&& pl) noexcept
		    : Serializable {this}
		{
			this->m_light.m_ambient_intensity  = std::move(pl.m_light.m_ambient_intensity);
			this->m_light.m_diffuse_intensity  = std::move(pl.m_light.m_diffuse_intensity);
			this->m_light.m_specular_intensity = std::move(pl.m_light.m_specular_intensity);
			this->m_light.m_pos                = std::move(pl.m_light.m_pos);
		}

		PointLight& PointLight::operator=(PointLight&& pl) noexcept
		{
			if (this != &pl)
			{
				this->m_light.m_ambient_intensity  = std::move(pl.m_light.m_ambient_intensity);
				this->m_light.m_diffuse_intensity  = std::move(pl.m_light.m_diffuse_intensity);
				this->m_light.m_specular_intensity = std::move(pl.m_light.m_specular_intensity);
				this->m_light.m_pos                = std::move(pl.m_light.m_pos);
			}

			return *this;
		}

		nlohmann::json PointLight::serialize()
		{
			nlohmann::json json = "{}"_json;

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

			return json;
		}

		void PointLight::deserialize(const nlohmann::json& json)
		{
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
		}
	} // namespace components
} // namespace galaxy