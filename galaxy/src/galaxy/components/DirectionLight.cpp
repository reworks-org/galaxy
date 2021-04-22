///
/// DirectionLight.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "DirectionLight.hpp"

namespace galaxy
{
	namespace components
	{
		DirectionLight::DirectionLight() noexcept
		    : Serializable {this}
		{
		}

		DirectionLight::DirectionLight(const nlohmann::json& json)
		    : Serializable {this}
		{
			deserialize(json);
		}

		DirectionLight::DirectionLight(DirectionLight&& dl) noexcept
		    : Serializable {this}
		{
			this->m_light.m_ambient_intensity  = std::move(dl.m_light.m_ambient_intensity);
			this->m_light.m_diffuse_intensity  = std::move(dl.m_light.m_diffuse_intensity);
			this->m_light.m_specular_intensity = std::move(dl.m_light.m_specular_intensity);
			this->m_light.m_dir                = std::move(dl.m_light.m_dir);
		}

		DirectionLight& DirectionLight::operator=(DirectionLight&& dl) noexcept
		{
			if (this != &dl)
			{
				this->m_light.m_ambient_intensity  = std::move(dl.m_light.m_ambient_intensity);
				this->m_light.m_diffuse_intensity  = std::move(dl.m_light.m_diffuse_intensity);
				this->m_light.m_specular_intensity = std::move(dl.m_light.m_specular_intensity);
				this->m_light.m_dir                = std::move(dl.m_light.m_dir);
			}

			return *this;
		}

		nlohmann::json DirectionLight::serialize()
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

			json["direction"]      = nlohmann::json::object();
			json["direction"]["x"] = m_light.m_dir.x;
			json["direction"]["y"] = m_light.m_dir.y;
			json["direction"]["z"] = m_light.m_dir.z;

			return json;
		}

		void DirectionLight::deserialize(const nlohmann::json& json)
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

			const auto& direction = json.at("direction");
			m_light.m_dir.x       = direction.at("x");
			m_light.m_dir.y       = direction.at("y");
			m_light.m_dir.z       = direction.at("z");
		}
	} // namespace components
} // namespace galaxy