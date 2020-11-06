///
/// PrimitiveVertex.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "PrimitiveVertex.hpp"

///
/// Core namespace.
///
namespace qs
{
	PrimitiveVertex::PrimitiveVertex() noexcept
	    : m_pos {0.0f, 0.0f}, m_colour {0, 0, 0, 255}
	{
	}

	PrimitiveVertex::PrimitiveVertex(const float x, const float y, pr::Colour& col) noexcept
	    : m_pos {x, y}
	{
		auto arr   = col.get_normalized();
		m_colour.x = arr[0];
		m_colour.y = arr[1];
		m_colour.z = arr[2];
		m_colour.w = arr[3];
	}

	PrimitiveVertex::PrimitiveVertex(const PrimitiveVertex& pv)
	{
		this->m_pos    = pv.m_pos;
		this->m_colour = pv.m_colour;
	}

	PrimitiveVertex::PrimitiveVertex(PrimitiveVertex&& pv)
	{
		this->m_pos    = std::move(pv.m_pos);
		this->m_colour = std::move(pv.m_colour);
	}

	PrimitiveVertex& PrimitiveVertex::operator=(const PrimitiveVertex& pv)
	{
		this->m_pos    = pv.m_pos;
		this->m_colour = pv.m_colour;

		return *this;
	}

	PrimitiveVertex& PrimitiveVertex::operator=(PrimitiveVertex&& pv)
	{
		if (this != &pv)
		{
			this->m_pos    = std::move(pv.m_pos);
			this->m_colour = std::move(pv.m_colour);
		}

		return *this;
	}
} // namespace qs