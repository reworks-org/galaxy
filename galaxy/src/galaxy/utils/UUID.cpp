///
/// UUID.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <iomanip>
#include <random>
#include <sstream>

#include "UUID.hpp"

namespace galaxy
{
	UUID::UUID()
	{
		std::mt19937_64                              engine {std::random_device()()};
		std::uniform_int_distribution<std::uint64_t> dist {0, 255};

		for (auto index = 0; index < 16; ++index)
		{
			m_uuid[index] = static_cast<unsigned char>(dist(engine));
		}

		m_uuid[6] = ((m_uuid[6] & 0x0f) | 0x40); // Version 4
		m_uuid[8] = ((m_uuid[8] & 0x3f) | 0x80); // Variant is 10

		std::stringstream hex;
		hex << std::hex << std::setfill('0');

		for (int i = 0; i < 16; ++i)
		{
			hex << std::setw(2) << static_cast<int>(m_uuid[i]);

			if (i == 3 || i == 5 || i == 7 || i == 9)
			{
				hex << '-';
			}
		}

		m_str = hex.str();
	}

	UUID::UUID(UUID&& g) noexcept
	{
		this->m_uuid = std::move(g.m_uuid);
		this->m_str  = std::move(g.m_str);
	}

	UUID& UUID::operator=(UUID&& g) noexcept
	{
		if (this != &g)
		{
			this->m_uuid = std::move(g.m_uuid);
			this->m_str  = std::move(g.m_str);
		}

		return *this;
	}

	UUID::UUID(const UUID& g) noexcept
	{
		this->m_uuid = g.m_uuid;
		this->m_str  = g.m_str;
	}

	UUID& UUID::operator=(const UUID& g) noexcept
	{
		if (this != &g)
		{
			this->m_uuid = g.m_uuid;
			this->m_str  = g.m_str;
		}

		return *this;
	}

	UUID::~UUID() noexcept
	{
	}

	std::size_t UUID::hash() noexcept
	{
		std::size_t h = 0;
		for (auto b : m_uuid)
		{
			h ^= static_cast<std::size_t>(b) + 0x9e3779b9 + (h << 6) + (h >> 2);
		}
		return h;
	}

	const std::string& UUID::str() const noexcept
	{
		return m_str;
	}

	bool UUID::operator==(const UUID& rhs) noexcept
	{
		return m_uuid == rhs.m_uuid;
	}

	bool UUID::operator!=(const UUID& rhs) noexcept
	{
		return !operator==(rhs);
	}
} // namespace galaxy
