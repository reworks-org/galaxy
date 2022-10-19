///
/// Colour.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Colour.hpp"

const constexpr auto COLOUR_DIVIDER = static_cast<float>(0xFF);

namespace galaxy
{
	namespace graphics
	{
		Colour::Colour() noexcept
			: m_red {255}
			, m_green {255}
			, m_blue {255}
			, m_alpha {255}
		{
		}

		Colour::Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a) noexcept
			: m_red {r}
			, m_green {g}
			, m_blue {b}
			, m_alpha {a}
		{
		}

		glm::vec4 Colour::normalized() noexcept
		{
			float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;

			if (m_red == 255)
			{
				r = 1.0f;
			}
			else if (m_red != 0)
			{
				r = static_cast<float>(m_red) / COLOUR_DIVIDER;
			}

			if (m_green == 255)
			{
				g = 1.0f;
			}
			else if (m_green != 0)
			{
				g = static_cast<float>(m_green) / COLOUR_DIVIDER;
			}

			if (m_blue == 255)
			{
				b = 1.0f;
			}
			else if (m_blue != 0)
			{
				b = static_cast<float>(m_blue) / COLOUR_DIVIDER;
			}

			if (m_alpha == 255)
			{
				a = 1.0f;
			}
			else if (m_alpha != 0)
			{
				a = static_cast<float>(m_alpha) / COLOUR_DIVIDER;
			}

			return {r, g, b, a};
		}
	} // namespace graphics
} // namespace galaxy