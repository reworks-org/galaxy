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
GALAXY_DISABLE_WARNING(4244)
#endif

namespace galaxy
{
	namespace graphics
	{
		Colour::Colour()
			: m_array {255, 255, 255, 255}
			, m_vec4 {1.0f, 1.0f, 1.0f, 1.0f}
		{
		}

		Colour::Colour(Colour&& c)
		{
			this->m_array = std::move(c.m_array);
			this->m_vec4  = std::move(c.m_vec4);
		}

		Colour::Colour(const Colour& c)
		{
			this->m_array = c.m_array;
			this->m_vec4  = c.m_vec4;
		}

		Colour& Colour::operator=(Colour&& c)
		{
			if (this != &c)
			{
				this->m_array = std::move(c.m_array);
				this->m_vec4  = std::move(c.m_vec4);
			}

			return *this;
		}

		Colour& Colour::operator=(const Colour& c)
		{
			if (this != &c)
			{
				this->m_array = c.m_array;
				this->m_vec4  = c.m_vec4;
			}

			return *this;
		}

		Colour::~Colour()
		{
		}

		void Colour::set_r(const std::uint8_t r)
		{
			m_array[0] = r;

			if (r == 255)
			{
				m_vec4.x = 1.0f;
			}
			else if (r == 0)
			{
				m_vec4.x = 0.0f;
			}
			else
			{
				m_vec4.x = static_cast<float>(r) / COLOUR_OFFSET;
			}
		}

		void Colour::set_g(const std::uint8_t g)
		{
			m_array[1] = g;

			if (g == 255)
			{
				m_vec4.y = 1.0f;
			}
			else if (g == 0)
			{
				m_vec4.y = 0.0f;
			}
			else
			{
				m_vec4.y = static_cast<float>(g) / COLOUR_OFFSET;
			}
		}

		void Colour::set_b(const std::uint8_t b)
		{
			m_array[2] = b;

			if (b == 255)
			{
				m_vec4.z = 1.0f;
			}
			else if (b == 0)
			{
				m_vec4.z = 0.0f;
			}
			else
			{
				m_vec4.z = static_cast<float>(b) / COLOUR_OFFSET;
			}
		}

		void Colour::set_a(const std::uint8_t a)
		{
			m_array[3] = a;

			if (a == 255)
			{
				m_vec4.w = 1.0f;
			}
			else if (a == 0)
			{
				m_vec4.w = 0.0f;
			}
			else
			{
				m_vec4.w = static_cast<float>(a) / COLOUR_OFFSET;
			}
		}

		void Colour::set_rgba(const glm::vec4& rgba)
		{
			m_vec4 = rgba;

			m_vec4.x = std::clamp(m_vec4.x, 0.0f, 1.0f);
			m_vec4.y = std::clamp(m_vec4.y, 0.0f, 1.0f);
			m_vec4.z = std::clamp(m_vec4.z, 0.0f, 1.0f);
			m_vec4.w = std::clamp(m_vec4.w, 0.0f, 1.0f);

			m_array[0] = m_vec4.x * COLOUR_OFFSET;
			m_array[1] = m_vec4.y * COLOUR_OFFSET;
			m_array[2] = m_vec4.z * COLOUR_OFFSET;
			m_array[3] = m_vec4.w * COLOUR_OFFSET;
		}

		std::array<std::uint8_t, 4>& Colour::array()
		{
			return m_array;
		}

		glm::vec4& Colour::vec4()
		{
			return m_vec4;
		}

		const std::array<std::uint8_t, 4>& Colour::array() const
		{
			return m_array;
		}

		const glm::vec4& Colour::vec4() const
		{
			return m_vec4;
		}
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif
