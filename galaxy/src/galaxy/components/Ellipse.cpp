///
/// Ellipse.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Ellipse.hpp"

namespace galaxy
{
	namespace components
	{
		Ellipse::Ellipse()
			: Serializable {}
		{
		}

		Ellipse::Ellipse(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		Ellipse::Ellipse(Ellipse&& e)
			: Serializable {}
		{
			this->m_shape = std::move(e.m_shape);
		}

		Ellipse& Ellipse::operator=(Ellipse&& e)
		{
			if (this != &e)
			{
				this->m_shape = std::move(e.m_shape);
			}

			return *this;
		}

		Ellipse::~Ellipse()
		{
		}

		nlohmann::json Ellipse::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["fragments"]   = m_shape.fragments();
			json["radii"]["x"]  = m_shape.radii().x;
			json["radii"]["y"]  = m_shape.radii().y;
			json["colour"]["r"] = m_shape.m_colour.r<std::uint8_t>();
			json["colour"]["g"] = m_shape.m_colour.g<std::uint8_t>();
			json["colour"]["b"] = m_shape.m_colour.b<std::uint8_t>();
			json["colour"]["a"] = m_shape.m_colour.a<std::uint8_t>();

			return json;
		}

		void Ellipse::deserialize(const nlohmann::json& json)
		{
			const auto& col = json.at("colour");
			m_shape.m_colour.set_r(col.at("r").get<std::uint8_t>());
			m_shape.m_colour.set_g(col.at("g").get<std::uint8_t>());
			m_shape.m_colour.set_b(col.at("b").get<std::uint8_t>());
			m_shape.m_colour.set_a(col.at("a").get<std::uint8_t>());

			glm::vec2   radii;
			const auto& rad = json.at("radii");
			radii.x         = rad.at("x");
			radii.y         = rad.at("y");

			m_shape.create(json.at("fragments"), radii);
		}
	} // namespace components
} // namespace galaxy
