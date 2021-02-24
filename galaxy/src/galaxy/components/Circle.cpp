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
		    : Serializable {this}
		{
			create(radius, fragments, colour);
		}

		Circle::Circle(const nlohmann::json& json)
		    : Serializable {this}
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
			m_fragments = static_cast<float>(fragments);
			m_colour    = colour;

			update();
		}

		void Circle::update()
		{
			// Thanks to https://stackoverflow.com/a/33859443
			// For help with maths.

			std::vector<graphics::PrimitiveVertex> vertexs;
			std::vector<unsigned int> indices;

			unsigned int count        = 0;
			constexpr float incr_stat = 2.0f * glm::pi<float>();
			const float increment     = incr_stat / m_fragments;
			for (float angle = 0.0f; angle <= (2.0f * glm::pi<float>()); angle += increment)
			{
				vertexs.emplace_back((m_radius * glm::cos(angle)) + m_radius, (m_radius * glm::sin(angle) + m_radius), m_colour);
				indices.push_back(count);

				count++;
			}

			m_vb.create<graphics::PrimitiveVertex>(vertexs);
			m_ib.create(indices);

			m_layout.add<graphics::PrimitiveVertex, meta::VAPosition>(2);
			m_layout.add<graphics::PrimitiveVertex, meta::VAColour>(4);

			m_va.create<graphics::PrimitiveVertex>(m_vb, m_ib, m_layout);
		}

		void Circle::set_radius(const float radius)
		{
			m_radius = radius;
		}

		void Circle::set_fragments(const float fragments)
		{
			m_fragments = fragments;
		}

		void Circle::change_colour(const graphics::Colour& col)
		{
			m_colour = col;
		}

		void Circle::bind() noexcept
		{
			m_va.bind();
		}

		void Circle::unbind() noexcept
		{
			m_va.unbind();
		}

		void Circle::set_opacity(const std::uint8_t opacity) noexcept
		{
			m_colour.m_alpha = opacity;
		}

		const std::uint8_t Circle::get_opacity() const noexcept
		{
			return m_colour.m_alpha;
		}

		const graphics::Colour& Circle::get_colour() const noexcept
		{
			return m_colour;
		}

		const float Circle::radius() const noexcept
		{
			return m_radius;
		}

		const float Circle::fragments() const noexcept
		{
			return m_fragments;
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