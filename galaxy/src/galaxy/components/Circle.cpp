///
/// Circle.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <vector>

#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>
#include <nlohmann/json.hpp>

#include "Circle.hpp"

namespace galaxy
{
	namespace components
	{
		Circle::Circle() noexcept
		    : Serializable {this}, m_radius {10.0f}, m_fragments {20}, m_colour {0, 0, 0, 255}
		{
		}

		Circle::Circle(const float radius, const unsigned int fragments, const graphics::Colour& colour)
		    : Serializable {this}, m_radius {10.0f}, m_fragments {20}, m_colour {0, 0, 0, 255}
		{
			create(radius, fragments, colour);
		}

		Circle::Circle(const nlohmann::json& json)
		    : Serializable {this}, m_radius {10.0f}, m_fragments {20}, m_colour {0, 0, 0, 255}
		{
			deserialize(json);
		}

		Circle::Circle(Circle&& c) noexcept
		    : VertexData {std::move(c)}, Serializable {this}
		{
			this->m_radius    = c.m_radius;
			this->m_fragments = c.m_fragments;
			this->m_colour    = std::move(c.m_colour);
		}

		Circle& Circle::operator=(Circle&& c) noexcept
		{
			if (this != &c)
			{
				graphics::VertexData::operator=(std::move(c));

				this->m_radius    = c.m_radius;
				this->m_fragments = c.m_fragments;
				this->m_colour    = std::move(c.m_colour);
			}

			return *this;
		}

		void Circle::create(const float radius, const unsigned int fragments, const graphics::Colour& colour)
		{
			m_radius    = radius;
			m_fragments = fragments;
			m_colour    = colour;

			// Thanks to https://stackoverflow.com/a/33859443.
			// For help with maths.

			std::vector<graphics::PrimitiveVertex> vertexs;
			std::vector<unsigned int> indices;

			unsigned int count        = 0;
			constexpr float incr_stat = 2.0f * glm::pi<float>();
			float increment           = incr_stat / static_cast<float>(fragments);
			for (float angle = 0.0f; angle <= (2.0f * glm::pi<float>()); angle += increment)
			{
				vertexs.emplace_back(radius * glm::cos(angle), radius * glm::sin(angle), colour);
				indices.push_back(count);

				count++;
			}

			m_vb.create<graphics::PrimitiveVertex>(vertexs);
			m_ib.create(indices);

			m_layout.add<graphics::PrimitiveVertex, meta::VAPosition>(2);
			m_layout.add<graphics::PrimitiveVertex, meta::VAColour>(4);

			m_va.create<graphics::PrimitiveVertex>(m_vb, m_ib, m_layout);
		}

		void Circle::bind() noexcept
		{
			m_va.bind();
		}

		void Circle::unbind() noexcept
		{
			m_va.unbind();
		}

		const float Circle::radius() const noexcept
		{
			return m_radius;
		}

		nlohmann::json Circle::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["radius"]      = m_radius;
			json["fragments"]   = m_fragments;

			json["colour"]      = nlohmann::json::object();
			json["colour"]["r"] = m_colour.m_red;
			json["colour"]["g"] = m_colour.m_green;
			json["colour"]["b"] = m_colour.m_blue;
			json["colour"]["a"] = m_colour.m_alpha;

			return json;
		}

		void Circle::deserialize(const nlohmann::json& json)
		{
			const auto colour = json.at("colour");
			create(json.at("radius"), json.at("fragments"), {colour.at("r"), colour.at("g"), colour.at("b"), colour.at("a")});
		}
	} // namespace components
} // namespace galaxy