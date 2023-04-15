///
/// Material.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Material.hpp"

namespace galaxy
{
	namespace physics
	{
		Material::Material() noexcept
			: density {1.0f}
			, friction {0.0f}
			, restitution {0.0f}
			, restitution_threshold {0.0f}
		{
		}

		Material::Material(const nlohmann::json& json) noexcept
		{
			deserialize(json);
		}

		Material::~Material() noexcept
		{
		}

		nlohmann::json Material::serialize()
		{
			nlohmann::json json           = "{}"_json;
			json["density"]               = density;
			json["friction"]              = friction;
			json["restitution"]           = restitution;
			json["restitution_threshold"] = restitution_threshold;

			return json;
		}

		void Material::deserialize(const nlohmann::json& json)
		{
			density               = json.at("density");
			friction              = json.at("friction");
			restitution           = json.at("restitution");
			restitution_threshold = json.at("restitution_threshold");
		}
	} // namespace physics
} // namespace galaxy