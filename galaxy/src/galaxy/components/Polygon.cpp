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
		Polygon::Polygon() noexcept
		    : Serializable {this}, m_colour {0, 0, 0, 255}
		{
		}

		Polygon::Polygon(const nlohmann::json& json)
		    : Serializable {this}, m_colour {0, 0, 0, 255}
		{
			deserialize(json);
		}

		Polygon::Polygon(Polygon&& p) noexcept
		    : VertexData {std::move(p)}, Serializable {this}, m_colour {0, 0, 0, 255}
		{
			this->m_points = p.m_points;
			this->m_colour = p.m_colour;
		}

		Polygon& Polygon::operator=(Polygon&& p) noexcept
		{
			if (this != &p)
			{
				graphics::VertexData::operator=(std::move(p));

				this->m_points = p.m_points;
				this->m_colour = p.m_colour;
			}

			return *this;
		}

		void Polygon::add_point(const float x, const float y)
		{
			m_points.emplace_back(x, y);
		}

		void Polygon::add_point(const std::pair<float, float>& point)
		{
			m_points.emplace_back(point);
		}

		void Polygon::create(const graphics::Colour& col)
		{
			m_colour = col;
			update();
		}

		void Polygon::update()
		{
			std::vector<graphics::PrimitiveVertex> vertexs;
			std::vector<unsigned int> indices;

			unsigned int count = 0;
			for (const auto& point : m_points)
			{
				vertexs.emplace_back(point.first, point.second, m_colour);
				indices.push_back(count);

				count++;
			}

			m_vb.create<graphics::PrimitiveVertex>(vertexs);
			m_ib.create(indices);

			m_layout.add<graphics::PrimitiveVertex, meta::VAPosition>(2);
			m_layout.add<graphics::PrimitiveVertex, meta::VAColour>(4);

			m_va.create<graphics::PrimitiveVertex>(m_vb, m_ib, m_layout);
		}

		void Polygon::change_colour(const graphics::Colour& col)
		{
			m_colour = col;
		}

		void Polygon::bind() noexcept
		{
			m_va.bind();
		}

		void Polygon::unbind() noexcept
		{
			m_va.unbind();
		}

		void Polygon::set_opacity(const std::uint8_t opacity) noexcept
		{
			m_colour.m_alpha = opacity;
		}

		const std::uint8_t Polygon::get_opacity() const noexcept
		{
			return m_colour.m_alpha;
		}

		const graphics::Colour& Polygon::get_colour() const noexcept
		{
			return m_colour;
		}

		const PointStorage& Polygon::get_points() const noexcept
		{
			return m_points;
		}

		nlohmann::json Polygon::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["points"] = nlohmann::json::array();
			for (const auto& point : m_points)
			{
				json["points"].push_back(nlohmann::json::object({{"x", point.first}, {"y", point.second}}));
			}

			json["colour"]      = nlohmann::json::object();
			json["colour"]["r"] = m_colour.m_red;
			json["colour"]["g"] = m_colour.m_green;
			json["colour"]["b"] = m_colour.m_blue;
			json["colour"]["a"] = m_colour.m_alpha;

			return json;
		}

		void Polygon::deserialize(const nlohmann::json& json)
		{
			m_points.clear();

			const auto& points = json.at("points");
			for (const auto& obj : points)
			{
				m_points.emplace_back(obj.at("x"), obj.at("y"));
			}

			const auto& colour = json.at("colour");
			m_colour           = {colour.at("r"), colour.at("g"), colour.at("b"), colour.at("a")};

			update();
		}
	} // namespace components
} // namespace galaxy