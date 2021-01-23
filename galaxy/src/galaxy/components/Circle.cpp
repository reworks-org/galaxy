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
		    : m_pos {0.0f, 0.0f}, m_radius {10.0f}
		{
		}

		Circle::Circle(const float x, const float y, const float radius, const unsigned int fragments, const graphics::Colour& colour)
		    : m_pos {0.0f, 0.0f}, m_radius {10.0f}
		{
			create(x, y, radius, fragments, colour);
		}

		Circle::Circle(const nlohmann::json& json)
		{
			const auto colour = json.at("colour");
			create(json.at("x"), json.at("y"), json.at("radius"), json.at("fragments"), {colour.at("r"), colour.at("g"), colour.at("b"), colour.at("a")});
		}

		Circle::Circle(Circle&& c) noexcept
		    : VertexData {std::move(c)}
		{
		}

		Circle& Circle::operator=(Circle&& c) noexcept
		{
			if (this != &c)
			{
				graphics::VertexData::operator=(std::move(c));
			}

			return *this;
		}

		void Circle::create(const float x, const float y, const float radius, const unsigned int fragments, const graphics::Colour& colour)
		{
			// Thanks to https://stackoverflow.com/a/33859443.
			// For help with maths.

			std::vector<graphics::PrimitiveVertex> vertexs;
			std::vector<unsigned int> indices;

			unsigned int count        = 0;
			constexpr float incr_stat = 2.0f * glm::pi<float>();
			float increment           = incr_stat / static_cast<float>(fragments);
			for (float angle = 0.0f; angle <= (2.0f * glm::pi<float>()); angle += increment)
			{
				vertexs.emplace_back(radius * glm::cos(angle) + x, radius * glm::sin(angle) + y, colour);
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

		const float Circle::get_x() const noexcept
		{
			return m_pos.x;
		}

		const float Circle::get_y() const noexcept
		{
			return m_pos.y;
		}

		const glm::vec2& Circle::get_xy() const noexcept
		{
			return m_pos;
		}

		const float Circle::radius() const noexcept
		{
			return m_radius;
		}
	} // namespace components
} // namespace galaxy