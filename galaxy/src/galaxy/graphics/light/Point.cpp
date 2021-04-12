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
	namespace light
	{
		Point::Point() noexcept
		    : Serializable {this}, m_pos {0.0f, 0.0f, 0.0f}, m_colour {1.0f, 1.0f, 1.0f}
		{
		}

		Point::Point(const nlohmann::json& json)
		    : Serializable {this}
		{
			deserialize(json);
		}

		nlohmann::json Point::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["point"]["x"] = m_pos.x;
			json["point"]["y"] = m_pos.y;
			json["point"]["z"] = m_pos.z;

			json["colour"]["r"] = m_colour.x;
			json["colour"]["g"] = m_colour.y;
			json["colour"]["b"] = m_colour.z;

			return json;
		}

		void Point::deserialize(const nlohmann::json& json)
		{
			const auto& point = json.at("point");
			m_pos.x           = point.at("x");
			m_pos.y           = point.at("y");
			m_pos.z           = point.at("z");

			const auto& colour = json.at("colour");
			m_colour.x         = colour.at("r");
			m_colour.y         = colour.at("g");
			m_colour.z         = colour.at("b");
		}
	} // namespace light
} // namespace galaxy