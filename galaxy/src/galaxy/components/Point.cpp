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
		    : m_size {0}
		{
		}

		Point::Point(const unsigned int size, const graphics::Colour& colour) noexcept
		    : m_size {0}
		{
			create(size, colour);
		}

		Point::Point(const nlohmann::json& json)
		{
			const auto colour = json.at("colour");
			create(json.at("size"), {colour.at("r"), colour.at("g"), colour.at("b"), colour.at("a")});
		}

		Point::Point(Point&& p) noexcept
		    : VertexData {std::move(p)}
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
			m_size = size;

			std::vector<graphics::PrimitiveVertex> vertexs;
			vertexs.emplace_back(0.0f, 0.0f, colour);

			m_vb.create<graphics::PrimitiveVertex>(vertexs);

			std::array<unsigned int, 1> arr = {0};
			m_ib.create(arr);

			m_layout.add<graphics::PrimitiveVertex, meta::VAPosition>(2);
			m_layout.add<graphics::PrimitiveVertex, meta::VAColour>(4);

			m_va.create<graphics::PrimitiveVertex>(m_vb, m_ib, m_layout);
		}

		void Point::bind() noexcept
		{
			m_va.bind();
		}

		void Point::unbind() noexcept
		{
			m_va.unbind();
		}

		void Point::set_size(const int size) noexcept
		{
			m_size = size;
		}

		const int Point::get_size() const noexcept
		{
			return m_size;
		}
	} // namespace components
} // namespace galaxy