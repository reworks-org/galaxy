///
/// Circle.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Circle.hpp"

namespace galaxy
{
	namespace components
	{
		Circle::Circle()
			: Serializable {}
		{
		}

		Circle::Circle(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		Circle::Circle(Circle&& c)
			: Serializable {}
		{
			this->m_shape = std::move(c.m_shape);
		}

		Circle& Circle::operator=(Circle&& c)
		{
			if (this != &c)
			{
				this->m_shape = std::move(c.m_shape);
			}

			return *this;
		}

		Circle::~Circle()
		{
		}

		nlohmann::json Circle::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["fragments"]   = m_shape.fragments();
			json["radius"]      = m_shape.radius();
			json["colour"]["r"] = m_shape.m_colour.r<std::uint8_t>();
			json["colour"]["g"] = m_shape.m_colour.g<std::uint8_t>();
			json["colour"]["b"] = m_shape.m_colour.b<std::uint8_t>();
			json["colour"]["a"] = m_shape.m_colour.a<std::uint8_t>();

			return json;
		}

		void Circle::deserialize(const nlohmann::json& json)
		{
			const auto& col = json.at("colour");
			m_shape.m_colour.set_r(col.at("r").get<std::uint8_t>());
			m_shape.m_colour.set_g(col.at("g").get<std::uint8_t>());
			m_shape.m_colour.set_b(col.at("b").get<std::uint8_t>());
			m_shape.m_colour.set_a(col.at("a").get<std::uint8_t>());

			m_shape.create(json.at("fragments"), json.at("radius"));
		}
	} // namespace components
} // namespace galaxy
