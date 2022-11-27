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
			: m_density {1.0f}
			, m_friction {0.5f}
			, m_restitution {0.0f}
			, m_restitution_threshold {0.5f}
		{
		}

		Material::Material(const nlohmann::json& json) noexcept
		{
			deserialize(json);
		}

		Material::Material(Material&& m) noexcept
		{
			this->m_density               = m.m_density;
			this->m_friction              = m.m_friction;
			this->m_restitution           = m.m_restitution;
			this->m_restitution_threshold = m.m_restitution_threshold;
		}

		Material::Material(const Material& m) noexcept
		{
			this->m_density               = m.m_density;
			this->m_friction              = m.m_friction;
			this->m_restitution           = m.m_restitution;
			this->m_restitution_threshold = m.m_restitution_threshold;
		}

		Material& Material::operator=(Material&& m) noexcept
		{
			if (this != &m)
			{
				this->m_density               = m.m_density;
				this->m_friction              = m.m_friction;
				this->m_restitution           = m.m_restitution;
				this->m_restitution_threshold = m.m_restitution_threshold;
			}

			return *this;
		}

		Material& Material::operator=(const Material& m) noexcept
		{
			if (this != &m)
			{
				this->m_density               = m.m_density;
				this->m_friction              = m.m_friction;
				this->m_restitution           = m.m_restitution;
				this->m_restitution_threshold = m.m_restitution_threshold;
			}

			return *this;
		}

		Material::~Material() noexcept
		{
		}

		nlohmann::json Material::serialize()
		{
			nlohmann::json json           = "{}"_json;
			json["density"]               = m_density;
			json["friction"]              = m_friction;
			json["restitution"]           = m_restitution;
			json["restitution_threshold"] = m_restitution_threshold;

			return json;
		}

		void Material::deserialize(const nlohmann::json& json)
		{
			m_density               = json.at("density");
			m_friction              = json.at("friction");
			m_restitution           = json.at("restitution");
			m_restitution_threshold = json.at("restitution_threshold");
		}
	} // namespace physics
} // namespace galaxy