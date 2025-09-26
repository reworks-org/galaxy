///
/// Colour.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "galaxy/math/Math.hpp"

#include "Colour.hpp"

namespace galaxy
{
	Colour::Colour() noexcept
		: m_red {0}
		, m_green {0}
		, m_blue {0}
		, m_alpha {OPAQUE}
	{
	}

	Colour::Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a) noexcept
	{
		this->r(r);
		this->g(g);
		this->b(b);
		this->a(a);
	}

	Colour::Colour(Colour&& c) noexcept
	{
		this->m_red   = c.m_red;
		this->m_green = c.m_green;
		this->m_blue  = c.m_blue;
		this->m_alpha = c.m_alpha;
	}

	Colour::Colour(const Colour& c) noexcept
	{
		this->m_red   = c.m_red;
		this->m_green = c.m_green;
		this->m_blue  = c.m_blue;
		this->m_alpha = c.m_alpha;
	}

	Colour& Colour::operator=(Colour&& c) noexcept
	{
		if (this != &c)
		{
			this->m_red   = c.m_red;
			this->m_green = c.m_green;
			this->m_blue  = c.m_blue;
			this->m_alpha = c.m_alpha;
		}

		return *this;
	}

	Colour& Colour::operator=(const Colour& c) noexcept
	{
		if (this != &c)
		{
			this->m_red   = c.m_red;
			this->m_green = c.m_green;
			this->m_blue  = c.m_blue;
			this->m_alpha = c.m_alpha;
		}

		return *this;
	}

	Colour::~Colour() noexcept
	{
	}

	void Colour::r(const std::uint8_t r) noexcept
	{
		m_red = std::clamp(r, LOWER, UPPER);
	}

	void Colour::g(const std::uint8_t g) noexcept
	{
		m_green = std::clamp(g, LOWER, UPPER);
	}

	void Colour::b(const std::uint8_t b) noexcept
	{
		m_blue = std::clamp(b, LOWER, UPPER);
	}

	void Colour::a(const std::uint8_t a) noexcept
	{
		m_alpha = std::clamp(a, LOWER, UPPER);
	}

	std::uint8_t Colour::r() const noexcept
	{
		return m_red;
	}

	std::uint8_t Colour::g() const noexcept
	{
		return m_green;
	}

	std::uint8_t Colour::b() const noexcept
	{
		return m_blue;
	}

	std::uint8_t Colour::a() const noexcept
	{
		return m_alpha;
	}

	void Colour::set_from_norm(const glm::vec4& rgba) noexcept
	{
		m_red   = std::clamp(static_cast<std::uint8_t>(rgba.x * OFFSET), LOWER, UPPER);
		m_green = std::clamp(static_cast<std::uint8_t>(rgba.y * OFFSET), LOWER, UPPER);
		m_blue  = std::clamp(static_cast<std::uint8_t>(rgba.z * OFFSET), LOWER, UPPER);
		m_alpha = std::clamp(static_cast<std::uint8_t>(rgba.w * OFFSET), LOWER, UPPER);
	}

	glm::vec4 Colour::normalize() noexcept
	{
		auto vec4 = glm::vec4();

		vec4.x = std::clamp(math::normalize(m_red, OFFSET), 0.0f, 1.0f);
		vec4.y = std::clamp(math::normalize(m_green, OFFSET), 0.0f, 1.0f);
		vec4.z = std::clamp(math::normalize(m_blue, OFFSET), 0.0f, 1.0f);
		vec4.w = std::clamp(math::normalize(m_alpha, OFFSET), 0.0f, 1.0f);

		return vec4;
	}

	std::array<std::uint8_t, 4> Colour::array() noexcept
	{
		return {m_red, m_green, m_blue, m_alpha};
	}
} // namespace galaxy
