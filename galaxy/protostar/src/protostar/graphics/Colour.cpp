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
	    : m_red(255), m_green(255), m_blue(255), m_alpha(255)
	{
	}

	Colour::Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b) noexcept
	    : m_red(r), m_green(g), m_blue(b), m_alpha(255)
	{
	}

	Colour::Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a) noexcept
	    : m_red(r), m_green(g), m_blue(b), m_alpha(a)
	{
	}

	decltype(auto) Colour::getNormalized() noexcept
	{
		std::array<float, 4> arr = {0.0f, 0.0f, 0.0f, 0.0f};

		// Avoids floating precision errors.
		if (m_red == 0)
		{
			arr[0] = 0.0f;
		}
		else if (m_red == 255)
		{
			arr[0] = 1.0f;
		}
		else
		{
			arr[0] = static_cast<float>(m_red) / static_cast<float>(0xFF);
		}

		if (m_green == 0)
		{
			arr[1] = 0.0f;
		}
		else if (m_green == 255)
		{
			arr[1] = 1.0f;
		}
		else
		{
			arr[1] = static_cast<float>(m_green) / static_cast<float>(0xFF);
		}

		if (m_blue == 0)
		{
			arr[2] = 0.0f;
		}
		else if (m_blue == 255)
		{
			arr[2] = 1.0f;
		}
		else
		{
			arr[2] = static_cast<float>(m_blue) / static_cast<float>(0xFF);
		}

		if (m_alpha == 0)
		{
			arr[3] = 0.0f;
		}
		else if (m_alpha == 255)
		{
			arr[3] = 1.0f;
		}
		else
		{
			arr[3] = static_cast<float>(m_alpha) / static_cast<float>(0xFF);
		}

		return std::move(arr);
	}
} // namespace pr