///
/// Light.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Light.hpp"

namespace galaxy
{
	namespace components
	{
		Light::Light()
			: Serializable {}
		{
			m_data.colour  = {1.0f, 1.0f, 1.0f, 1.0f};
			m_data.falloff = {0.4f, 0.7f, 40.0f};
			m_data.pos     = {0.0f, 0.0f};
			m_data.depth   = 0.075f;
		}

		Light::Light(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		Light::Light(Light&& l)
			: Serializable {}
		{
			this->m_data.colour  = std::move(l.m_data.colour);
			this->m_data.falloff = std::move(l.m_data.falloff);
			this->m_data.pos     = std::move(l.m_data.pos);
			this->m_data.depth   = l.m_data.depth;
		}

		Light& Light::operator=(Light&& l)
		{
			if (this != &l)
			{
				this->m_data.colour  = std::move(l.m_data.colour);
				this->m_data.falloff = std::move(l.m_data.falloff);
				this->m_data.pos     = std::move(l.m_data.pos);
				this->m_data.depth   = l.m_data.depth;
			}

			return *this;
		}

		Light::~Light()
		{
		}

		nlohmann::json Light::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["colour"]["r"] = m_data.colour.x;
			json["colour"]["g"] = m_data.colour.y;
			json["colour"]["b"] = m_data.colour.z;
			json["colour"]["a"] = m_data.colour.w;

			json["falloff"]["x"] = m_data.falloff.x;
			json["falloff"]["y"] = m_data.falloff.y;
			json["falloff"]["z"] = m_data.falloff.z;

			json["pos"]["x"] = m_data.pos.x;
			json["pos"]["y"] = m_data.pos.y;

			json["depth"] = m_data.depth;

			return json;
		}

		void Light::deserialize(const nlohmann::json& json)
		{
			const auto& colour = json.at("colour");
			m_data.colour.x    = colour.at("x");
			m_data.colour.y    = colour.at("y");
			m_data.colour.z    = colour.at("z");
			m_data.colour.w    = colour.at("w");

			const auto& falloff = json.at("falloff");
			m_data.falloff.x    = falloff.at("x");
			m_data.falloff.y    = falloff.at("y");
			m_data.falloff.z    = falloff.at("z");

			const auto& pos = json.at("pos");
			m_data.pos.x    = pos.at("x");
			m_data.pos.y    = pos.at("y");

			m_data.depth = json.at("depth");
		}
	} // namespace components
} // namespace galaxy