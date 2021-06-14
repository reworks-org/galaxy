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
		    : m_red {255}, m_green {255}, m_blue {255}, m_alpha {255}
		{
		}

		Colour::Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a) noexcept
		    : m_red {r}, m_green {g}, m_blue {b}, m_alpha {a}
		{
		}

		std::array<float, 4> Colour::normalized() noexcept
		{
			// clang-format off
			std::array<float, 4> arr = 
			{
				r_normal(),
				g_normal(),
				b_normal(),
				a_normal()
			};
			// clang-format on

			return arr;
		}

		const float Colour::r_normal() noexcept
		{
			if (m_red == 0)
			{
				return 0.0f;
			}
			else if (m_red == 255)
			{
				return 1.0f;
			}
			else
			{
				return static_cast<float>(m_red) / static_cast<float>(0xFF);
			}
		}

		const float Colour::g_normal() noexcept
		{
			if (m_green == 0)
			{
				return 0.0f;
			}
			else if (m_green == 255)
			{
				return 1.0f;
			}
			else
			{
				return static_cast<float>(m_green) / static_cast<float>(0xFF);
			}
		}

		const float Colour::b_normal() noexcept
		{
			if (m_blue == 0)
			{
				return 0.0f;
			}
			else if (m_blue == 255)
			{
				return 1.0f;
			}
			else
			{
				return static_cast<float>(m_blue) / static_cast<float>(0xFF);
			}
		}

		const float Colour::a_normal() noexcept
		{
			if (m_alpha == 0)
			{
				return 0.0f;
			}
			else if (m_alpha == 255)
			{
				return 1.0f;
			}
			else
			{
				return static_cast<float>(m_alpha) / static_cast<float>(0xFF);
			}
		}
	} // namespace graphics
} // namespace galaxy