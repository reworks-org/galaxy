///
/// Sprite2D.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <cstdarg>

#include "Sprite2D.hpp"

///
/// Core namespace.
///
namespace qs
{
	Sprite2D::Sprite2D() noexcept
		:Transform()
	{
	}
	
	Sprite2D::~Sprite2D() noexcept
	{
	}

	void Sprite2D::addLayout(const std::initializer_list<int>& strides) noexcept
	{
		for (const int& f : strides)
		{
			m_layout.add<float>(f);
		}
	}

	void Sprite2D::create() noexcept
	{
		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);
	}
}