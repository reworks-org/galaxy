///
/// Polygon.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Polygon.hpp"

namespace galaxy
{
	namespace components
	{
		Polygon::Polygon()
			: Serializable {}
		{
		}

		Polygon::Polygon(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		Polygon::Polygon(Polygon&& p)
			: Serializable {}
		{
			this->m_shape = std::move(p.m_shape);
		}

		Polygon& Polygon::operator=(Polygon&& p)
		{
			if (this != &p)
			{
				this->m_shape = std::move(p.m_shape);
			}

			return *this;
		}

		Polygon::~Polygon()
		{
		}

		nlohmann::json Polygon::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["colour"]["r"] = m_shape.m_colour.r<std::uint8_t>();
			json["colour"]["g"] = m_shape.m_colour.g<std::uint8_t>();
			json["colour"]["b"] = m_shape.m_colour.b<std::uint8_t>();
			json["colour"]["a"] = m_shape.m_colour.a<std::uint8_t>();

			json["points"] = nlohmann::json::array();
			for (const auto& point : m_shape.points())
			{
				auto obj = nlohmann::json::object();
				obj["x"] = point.x;
				obj["y"] = point.y;

				json["points"].push_back(obj);
			}

			return json;
		}

		void Polygon::deserialize(const nlohmann::json& json)
		{
			const auto& col = json.at("colour");
			m_shape.m_colour.r(col.at("r").get<std::uint8_t>());
			m_shape.m_colour.g(col.at("g").get<std::uint8_t>());
			m_shape.m_colour.b(col.at("b").get<std::uint8_t>());
			m_shape.m_colour.a(col.at("a").get<std::uint8_t>());

			meta::vector<glm::vec2> points;

			const auto& pa = json.at("points");
			for (const auto& point : pa)
			{
				points.emplace_back(point.at("x"), point.at("y"));
			}

			m_shape.create(points);
		}
	} // namespace components
} // namespace galaxy
