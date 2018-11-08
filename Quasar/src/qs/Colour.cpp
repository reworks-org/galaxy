///
/// Colour.cpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <cmath>
#include <algorithm>

#include "Colour.hpp"

namespace qs
{
	///
	/// Helper function.
	///
	Uint8 fromFloat(float f)
	{
		// Makes sure the float is properly converted to a byte in the correct range.
		return (f >= 1.0 ? 255 : (f <= 0.0 ? 0 : static_cast<Uint8>(std::floor(f * 256.0))));
	}

	Colour::Colour() noexcept
		:m_red(0.0f), m_green(0.0f), m_blue(0.0f), m_alpha(1.0f)
	{
	}

	Colour::Colour(float r, float g, float b, float a) noexcept
	{
		// Ensure values are correct.
		m_red = std::clamp(r, 0.0f, 1.0f);
		m_green = std::clamp(g, 0.0f, 1.0f);
		m_blue = std::clamp(b, 0.0f, 1.0f);
		m_alpha = std::clamp(a, 0.0f, 1.0f);
	}

	Colour::Colour(SDL_Colour colour) noexcept
	{
		// Make sure math is done with division, not integer division.
		m_red = static_cast<float>(colour.r) / 255.0f;
		m_green = static_cast<float>(colour.g) / 255.0f;
		m_blue = static_cast<float>(colour.b) / 255.0f;
		m_alpha = static_cast<float>(colour.a) / 255.0f;
	}

	SDL_Colour Colour::asSDL() noexcept
	{
		SDL_Colour col;

		// Makes sure floats are properly converted into 0..255 bytes.
		col.r = fromFloat(m_red);
		col.g = fromFloat(m_green);
		col.b = fromFloat(m_blue);
		col.a = fromFloat(m_alpha);

		return col;
	}
}