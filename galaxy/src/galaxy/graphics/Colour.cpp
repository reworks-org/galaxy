///
/// Colour.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/platform/Pragma.hpp"

#include "Colour.hpp"

const constexpr auto COLOUR_OFFSET = static_cast<float>(0xFF);

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26467)
#endif

namespace galaxy
{
	namespace graphics
	{
		Colour::Colour()
			: m_red {255}
			, m_green {255}
			, m_blue {255}
			, m_alpha {255}
		{
		}

		Colour::Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a)
			: m_red {r}
			, m_green {g}
			, m_blue {b}
			, m_alpha {a}
		{
		}

		Colour::~Colour()
		{
		}

		void Colour::set_from_normalized(const float r, const float g, const float b, const float a)
		{
			m_red   = static_cast<std::uint8_t>(std::max(r, 0.0f) * COLOUR_OFFSET);
			m_green = static_cast<std::uint8_t>(std::max(g, 0.0f) * COLOUR_OFFSET);
			m_blue  = static_cast<std::uint8_t>(std::max(b, 0.0f) * COLOUR_OFFSET);
			m_alpha = static_cast<std::uint8_t>(std::max(a, 0.0f) * COLOUR_OFFSET);
		}

		std::array<float, 4> Colour::to_array()
		{
			float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;

			if (m_red == 255)
			{
				r = 1.0f;
			}
			else if (m_red != 0)
			{
				r = static_cast<float>(m_red) / COLOUR_OFFSET;
			}

			if (m_green == 255)
			{
				g = 1.0f;
			}
			else if (m_green != 0)
			{
				g = static_cast<float>(m_green) / COLOUR_OFFSET;
			}

			if (m_blue == 255)
			{
				b = 1.0f;
			}
			else if (m_blue != 0)
			{
				b = static_cast<float>(m_blue) / COLOUR_OFFSET;
			}

			if (m_alpha == 255)
			{
				a = 1.0f;
			}
			else if (m_alpha != 0)
			{
				a = static_cast<float>(m_alpha) / COLOUR_OFFSET;
			}

			return {r, g, b, a};
		}

		glm::vec4 Colour::to_vec4()
		{
			const auto arr = to_array();
			return {arr[0], arr[1], arr[2], arr[3]};
		}
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif