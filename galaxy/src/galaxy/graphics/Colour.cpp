///
/// Colour.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Colour.hpp"

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
			static const constexpr auto DIVIDER = static_cast<float>(0xFF);

			float r, g, b, a;

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
				r = static_cast<float>(m_red) / DIVIDER;
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
				g = static_cast<float>(m_green) / DIVIDER;
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
				b = static_cast<float>(m_blue) / DIVIDER;
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
				a = static_cast<float>(m_alpha) / DIVIDER;
			}

			return {r, g, b, a};
		}
	} // namespace graphics
} // namespace galaxy