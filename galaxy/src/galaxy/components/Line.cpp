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
		Line::Line(const graphics::Colour& col, const float x1, const float y1, const float x2, const float y2) noexcept
		{
			create(col, x1, y1, x2, y2);
		}

		Line::Line(const nlohmann::json& json)
		{
			const auto colour = json.at("colour");
			create({colour.at("r"), colour.at("g"), colour.at("b"), colour.at("a")}, json.at("x1"), json.at("y1"), json.at("x2"), json.at("y2"));
		}

		Line::Line(Line&& l) noexcept
		    : VertexData {std::move(l)}
		{
		}

		Line& Line::operator=(Line&& l) noexcept
		{
			if (this != &l)
			{
				graphics::VertexData::operator=(std::move(l));
			}

			return *this;
		}

		void Line::create(const graphics::Colour& col, const float x1, const float y1, const float x2, const float y2)
		{
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

		void Line::bind() noexcept
		{
			m_va.bind();
		}

		void Line::unbind() noexcept
		{
			m_va.unbind();
		}
	} // namespace components
} // namespace galaxy