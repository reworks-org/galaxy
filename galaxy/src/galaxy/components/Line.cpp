///
/// Line.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Line.hpp"

namespace galaxy
{
	namespace components
	{
		Line::Line() noexcept
		    : Serializable {this}, m_point_a {0.0f, 0.0f}, m_point_b {0.0f, 0.0f}, m_colour {0, 0, 0, 255}
		{
		}

		Line::Line(const graphics::Colour& col, const float x1, const float y1, const float x2, const float y2) noexcept
		    : Serializable {this}
		{
			create(col, x1, y1, x2, y2);
		}

		Line::Line(const nlohmann::json& json)
		    : Serializable {this}
		{
			deserialize(json);
		}

		Line::Line(Line&& l) noexcept
		    : VertexData {std::move(l)}, Serializable {this}
		{
			this->m_point_a = std::move(l.m_point_a);
			this->m_point_b = std::move(l.m_point_b);
			this->m_colour  = std::move(l.m_colour);
		}

		Line& Line::operator=(Line&& l) noexcept
		{
			if (this != &l)
			{
				graphics::VertexData::operator=(std::move(l));

				this->m_point_a = std::move(l.m_point_a);
				this->m_point_b = std::move(l.m_point_b);
				this->m_colour  = std::move(l.m_colour);
			}

			return *this;
		}

		void Line::create(const graphics::Colour& col, const float x1, const float y1, const float x2, const float y2)
		{
			m_point_a = {x1, y1};
			m_point_b = {x2, y2};
			m_colour  = col;

			std::vector<graphics::PrimitiveVertex> vertexs;
			vertexs.emplace_back(x1, y1, col);
			vertexs.emplace_back(x2, y2, col);

			m_vb.create<graphics::PrimitiveVertex>(vertexs);

			std::array<unsigned int, 2> arr = {0, 1};
			m_ib.create(arr);

			m_layout.add<graphics::PrimitiveVertex, meta::VAPosition>(2);
			m_layout.add<graphics::PrimitiveVertex, meta::VAColour>(4);

			m_va.create<graphics::PrimitiveVertex>(m_vb, m_ib, m_layout);
		}

		void Line::change_colour(const graphics::Colour& col)
		{
			create(col, m_point_a.x, m_point_a.y, m_point_b.x, m_point_b.y);
		}

		void Line::bind() noexcept
		{
			m_va.bind();
		}

		void Line::unbind() noexcept
		{
			m_va.unbind();
		}

		const graphics::Colour& Line::get_colour() const noexcept
		{
			return m_colour;
		}

		nlohmann::json Line::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["colour"]      = nlohmann::json::object();
			json["colour"]["r"] = m_colour.m_red;
			json["colour"]["g"] = m_colour.m_green;
			json["colour"]["b"] = m_colour.m_blue;
			json["colour"]["a"] = m_colour.m_alpha;

			json["x1"] = m_point_a.x;
			json["y1"] = m_point_a.y;
			json["x2"] = m_point_b.x;
			json["y2"] = m_point_b.y;

			return json;
		}

		void Line::deserialize(const nlohmann::json& json)
		{
			const auto colour = json.at("colour");
			create({colour.at("r"), colour.at("g"), colour.at("b"), colour.at("a")}, json.at("x1"), json.at("y1"), json.at("x2"), json.at("y2"));
		}
	} // namespace components
} // namespace galaxy