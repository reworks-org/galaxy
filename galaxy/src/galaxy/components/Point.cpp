///
/// Point.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Point.hpp"

namespace galaxy
{
	namespace components
	{
		Point::Point()
			: Serializable {}
		{
		}

		Point::Point(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		Point::Point(Point&& p)
			: Serializable {}
		{
			this->m_shape = std::move(p.m_shape);
		}

		Point& Point::operator=(Point&& p)
		{
			if (this != &p)
			{
				this->m_shape = std::move(p.m_shape);
			}

			return *this;
		}

		Point::~Point()
		{
		}

		nlohmann::json Point::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["pos"]["x"]    = m_shape.pos().x;
			json["pos"]["y"]    = m_shape.pos().y;
			json["colour"]["r"] = m_shape.m_colour.r<std::uint8_t>();
			json["colour"]["g"] = m_shape.m_colour.g<std::uint8_t>();
			json["colour"]["b"] = m_shape.m_colour.b<std::uint8_t>();
			json["colour"]["a"] = m_shape.m_colour.a<std::uint8_t>();

			return json;
		}

		void Point::deserialize(const nlohmann::json& json)
		{
			const auto& col = json.at("colour");
			m_shape.m_colour.r(col.at("r").get<std::uint8_t>());
			m_shape.m_colour.g(col.at("g").get<std::uint8_t>());
			m_shape.m_colour.b(col.at("b").get<std::uint8_t>());
			m_shape.m_colour.a(col.at("a").get<std::uint8_t>());

			glm::vec2   pos;
			const auto& pj = json.at("pos");
			pos.x          = pj.at("x");
			pos.y          = pj.at("y");

			m_shape.create(pos);
		}
	} // namespace components
} // namespace galaxy
