///
/// Colour.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "Colour.hpp"

///
/// Core namespace.
///
namespace protostar
{
	Colour::Colour()
		:m_red(0), m_green(0), m_blue(0), m_alpha(255)
	{
	}

	Colour::Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a)
		:m_red(r), m_green(g), m_blue(b), m_alpha(a)
	{
	}
}