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
		Point::Point() noexcept
		    : Serializable {this}, m_size {0}, m_colour {0, 0, 0, 255}
		{
		}

		Point::Point(const unsigned int size, const graphics::Colour& colour) noexcept
		    : Serializable {this}, m_size {0}, m_colour {0, 0, 0, 255}
		{
			create(size, colour);
		}

		Point::Point(const nlohmann::json& json)
		    : Serializable {this}
		{
			deserialize(json);
		}

		Point::Point(Point&& p) noexcept
		    : VertexData {std::move(p)}, Serializable {this}
		{
			this->m_size = p.m_size;
		}

		Point& Point::operator=(Point&& p) noexcept
		{
			if (this != &p)
			{
				graphics::VertexData::operator=(std::move(p));

				this->m_size = p.m_size;
			}

			return *this;
		}

		void Point::create(const unsigned int size, const graphics::Colour& colour)
		{
			m_size   = size;
			m_colour = colour;

			update();
		}

		void Point::update()
		{
			std::vector<graphics::PrimitiveVertex> vertexs;
			vertexs.emplace_back(0.0f, 0.0f, m_colour);

			m_vb.create<graphics::PrimitiveVertex>(vertexs);

			std::array<unsigned int, 1> arr = {0};
			m_ib.create(arr);

			m_layout.add<graphics::PrimitiveVertex, meta::VAPosition>(2);
			m_layout.add<graphics::PrimitiveVertex, meta::VAColour>(4);

			m_va.create<graphics::PrimitiveVertex>(m_vb, m_ib, m_layout);
		}

		void Point::change_colour(const graphics::Colour& col)
		{
			m_colour = col;
		}

		void Point::bind() noexcept
		{
			m_va.bind();
		}

		void Point::unbind() noexcept
		{
			m_va.unbind();
		}

		const graphics::Colour& Point::get_colour() const noexcept
		{
			return m_colour;
		}

		void Point::set_size(const int size) noexcept
		{
			m_size = size;
		}

		const int Point::get_size() const noexcept
		{
			return m_size;
		}

		nlohmann::json Point::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["size"]        = m_size;

			json["colour"]      = nlohmann::json::object();
			json["colour"]["r"] = m_colour.m_red;
			json["colour"]["g"] = m_colour.m_green;
			json["colour"]["b"] = m_colour.m_blue;
			json["colour"]["a"] = m_colour.m_alpha;

			return json;
		}

		void Point::deserialize(const nlohmann::json& json)
		{
			const auto colour = json.at("colour");
			create(json.at("size"), {colour.at("r"), colour.at("g"), colour.at("b"), colour.at("a")});
		}
	} // namespace components
} // namespace galaxy