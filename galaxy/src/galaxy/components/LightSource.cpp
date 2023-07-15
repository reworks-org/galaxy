///
/// LightSource.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "LightSource.hpp"

namespace galaxy
{
	namespace components
	{
		LightSource::LightSource()
			: Serializable {}
		{
			m_light.colour  = {1.0f, 1.0f, 1.0f, 1.0f};
			m_light.falloff = {0.4f, 0.7f, 30.0f};
			m_light.pos     = {0.0f, 0.0f};
			m_light.depth   = 0.075f;
		}

		LightSource::LightSource(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		LightSource::LightSource(LightSource* ptr)
			: Serializable {}
		{
			const auto& copy = *ptr;

			this->m_light.colour  = copy.m_light.colour;
			this->m_light.falloff = copy.m_light.falloff;
			this->m_light.pos     = copy.m_light.pos;
			this->m_light.depth   = copy.m_light.depth;
		}

		LightSource::LightSource(LightSource&& l)
			: Serializable {}
		{
			this->m_light.colour  = std::move(l.m_light.colour);
			this->m_light.falloff = std::move(l.m_light.falloff);
			this->m_light.pos     = std::move(l.m_light.pos);
			this->m_light.depth   = l.m_light.depth;
		}

		LightSource& LightSource::operator=(LightSource&& l)
		{
			if (this != &l)
			{
				this->m_light.colour  = std::move(l.m_light.colour);
				this->m_light.falloff = std::move(l.m_light.falloff);
				this->m_light.pos     = std::move(l.m_light.pos);
				this->m_light.depth   = l.m_light.depth;
			}

			return *this;
		}

		LightSource::~LightSource()
		{
		}

		nlohmann::json LightSource::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["colour"]["r"] = m_light.colour.x;
			json["colour"]["g"] = m_light.colour.y;
			json["colour"]["b"] = m_light.colour.z;
			json["colour"]["a"] = m_light.colour.w;

			json["falloff"]["x"] = m_light.falloff.x;
			json["falloff"]["y"] = m_light.falloff.y;
			json["falloff"]["z"] = m_light.falloff.z;

			json["pos"]["x"] = m_light.pos.x;
			json["pos"]["y"] = m_light.pos.y;

			json["depth"] = m_light.depth;

			return json;
		}

		void LightSource::deserialize(const nlohmann::json& json)
		{
			const auto& colour = json.at("colour");
			m_light.colour.x   = colour.at("r");
			m_light.colour.y   = colour.at("g");
			m_light.colour.z   = colour.at("b");
			m_light.colour.w   = colour.at("a");

			const auto& falloff = json.at("falloff");
			m_light.falloff.x   = falloff.at("x");
			m_light.falloff.y   = falloff.at("y");
			m_light.falloff.z   = falloff.at("z");

			const auto& pos = json.at("pos");
			m_light.pos.x   = pos.at("x");
			m_light.pos.y   = pos.at("y");

			m_light.depth = json.at("depth");
		}
	} // namespace components
} // namespace galaxy