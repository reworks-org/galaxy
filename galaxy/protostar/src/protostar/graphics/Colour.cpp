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
namespace pr
{
	Colour::Colour() noexcept
		:m_red(0), m_green(0), m_blue(0), m_alpha(255)
	{
	}

	Colour::Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a) noexcept
		:m_red(r), m_green(g), m_blue(b), m_alpha(a)
	{
	}

	std::array<float, 4> Colour::asFloats() noexcept
	{
		std::array<float, 4> arr = { static_cast<float>(m_red), static_cast<float>(m_green),
			static_cast<float>(m_blue), static_cast<float>(m_alpha) };
		return std::move(arr);
	}

	std::array<int, 4> Colour::asIntegers() noexcept
	{
		std::array<int, 4> arr = { static_cast<int>(m_red), static_cast<int>(m_green),
			static_cast<int>(m_blue), static_cast<int>(m_alpha) };
		return std::move(arr);
	}

	std::array<float, 4> Colour::asGLColour() noexcept
	{
		float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;

		// Avoids floating precision errors.
		if (m_red == 0)
		{
			r = 0.0f;
		}
		else if (m_red == 255)
		{
			r = 1.0f;
		}
		else
		{
			r = static_cast<float>(m_red) / static_cast<float>(0xFF);
		}

		if (m_green == 0)
		{
			g = 0.0f;
		}
		else if (m_green == 255)
		{
			g = 1.0f;
		}
		else
		{
			g = static_cast<float>(m_green) / static_cast<float>(0xFF);
		}

		if (m_blue == 0)
		{
			b = 0.0f;
		}
		else if (m_blue == 255)
		{
			b = 1.0f;
		}
		else
		{
			b = static_cast<float>(m_blue) / static_cast<float>(0xFF);
		}

		if (m_alpha == 0)
		{
			a = 0.0f;
		}
		else if (m_alpha == 255)
		{
			a = 1.0f;
		}
		else
		{
			a = static_cast<float>(m_alpha) / static_cast<float>(0xFF);
		}

		return std::move(std::array<float, 4>{r, g, b, a});
	}
}