///
/// Ellipse.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <vector>

#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>
#include <nlohmann/json.hpp>

#include "Ellipse.hpp"

namespace galaxy
{
	namespace components
	{
		Ellipse::Ellipse() noexcept
		    : Serializable {this}, m_radius {10.0f, 15.0f}, m_fragments {20}, m_colour {0, 0, 0, 255}
		{
		}

		Ellipse::Ellipse(const glm::vec2& radius, const unsigned int fragments, const graphics::Colour& colour)
		    : Serializable {this}
		{
			create(radius, fragments, colour);
		}

		Ellipse::Ellipse(const nlohmann::json& json)
		    : Serializable {this}
		{
			deserialize(json);
		}

		Ellipse::Ellipse(Ellipse&& e) noexcept
		    : VertexData {std::move(e)}, Serializable {this}
		{
			this->m_radius    = e.m_radius;
			this->m_fragments = e.m_fragments;
			this->m_colour    = std::move(e.m_colour);
			this->m_vertexs   = std::move(e.m_vertexs);
		}

		Ellipse& Ellipse::operator=(Ellipse&& e) noexcept
		{
			if (this != &e)
			{
				graphics::VertexData::operator=(std::move(e));

				this->m_radius    = e.m_radius;
				this->m_fragments = e.m_fragments;
				this->m_colour    = std::move(e.m_colour);
				this->m_vertexs   = std::move(e.m_vertexs);
			}

			return *this;
		}

		void Ellipse::create(const glm::vec2& radius, const unsigned int fragments, const graphics::Colour& colour)
		{
			m_radius    = radius;
			m_fragments = static_cast<float>(fragments);
			m_colour    = colour;

			update();
		}

		void Ellipse::update()
		{
			// Thanks to https://stackoverflow.com/a/34735255
			// For help with maths.

			m_vertexs.clear();
			std::vector<unsigned int> indices;

			const float theta  = 2.0f * glm::pi<float>() / m_fragments;
			const float cosine = std::cosf(theta);
			const float sine   = std::sinf(theta);

			float temp = 0.0f;
			float x    = 1.0f;
			float y    = 0.0f;

			unsigned int count = 0;
			for (auto i = 0; i < std::floor(m_fragments); i++)
			{
				m_vertexs.emplace_back((x * m_radius.x) + m_radius.x, (y * m_radius.y) + m_radius.y, m_colour);
				indices.push_back(count);
				count++;

				temp = x;
				x    = cosine * x - sine * y;
				y    = sine * temp + cosine * y;
			}

			m_vb.create<graphics::PrimitiveVertex>(m_vertexs);
			m_ib.create(indices);

			m_layout.add<graphics::PrimitiveVertex, meta::VAPosition>(2);
			m_layout.add<graphics::PrimitiveVertex, meta::VAColour>(4);

			m_va.create<graphics::PrimitiveVertex>(m_vb, m_ib, m_layout);
		}

		void Ellipse::set_radius(const glm::vec2& radius)
		{
			m_radius = radius;
		}

		void Ellipse::set_fragments(const float fragments)
		{
			m_fragments = fragments;
		}

		void Ellipse::change_colour(const graphics::Colour& col)
		{
			m_colour = col;
		}

		void Ellipse::bind() noexcept
		{
			m_va.bind();
		}

		void Ellipse::unbind() noexcept
		{
			m_va.unbind();
		}

		void Ellipse::set_opacity(const std::uint8_t opacity) noexcept
		{
			m_colour.m_alpha = opacity;
		}

		const std::uint8_t Ellipse::get_opacity() const noexcept
		{
			return m_colour.m_alpha;
		}

		const graphics::Colour& Ellipse::get_colour() const noexcept
		{
			return m_colour;
		}

		const glm::vec2& Ellipse::radius() const noexcept
		{
			return m_radius;
		}

		const float Ellipse::fragments() const noexcept
		{
			return m_fragments;
		}

		const std::vector<graphics::PrimitiveVertex>& Ellipse::get_vertexs() const noexcept
		{
			return m_vertexs;
		}

		nlohmann::json Ellipse::serialize()
		{
			nlohmann::json json       = "{}"_json;
			json["horizontal-radius"] = m_radius.x;
			json["vertical-radius"]   = m_radius.y;
			json["fragments"]         = m_fragments;

			json["colour"]      = nlohmann::json::object();
			json["colour"]["r"] = m_colour.m_red;
			json["colour"]["g"] = m_colour.m_green;
			json["colour"]["b"] = m_colour.m_blue;
			json["colour"]["a"] = m_colour.m_alpha;

			return json;
		}

		void Ellipse::deserialize(const nlohmann::json& json)
		{
			const auto colour = json.at("colour");

			glm::vec2 radius;
			radius.x = json.at("horizontal-radius");
			radius.y = json.at("vertical-radius");
			create(radius, json.at("fragments"), {colour.at("r"), colour.at("g"), colour.at("b"), colour.at("a")});
		}
	} // namespace components
} // namespace galaxy