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

	std::array<float, 4> Colour::asFloats() noexcept
	{
		std::array<float, 4> arr = { m_red, m_green, m_blue, m_alpha };
		return std::move(arr);
	}

	std::array<int, 4> Colour::asIntegers() noexcept
	{
		std::array<int, 4> arr = { m_red, m_green, m_blue, m_alpha };
		return std::move(arr);
	}
}