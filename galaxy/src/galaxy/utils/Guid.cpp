///
/// Guid.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sstream>

#include "galaxy/math/Random.hpp"

#include "Guid.hpp"

static constexpr const char* const s_chars {"abcdef"};

namespace galaxy
{
	namespace utils
	{
		Guid::Guid()
			: m_id {""}
		{
			math::Random rng;

			m_id += s_chars[static_cast<int>(std::floor(rng.gen<float>(0.0f, 1.0f) * 6))];

			for (auto i = 1; i < 32; i++)
			{
				const auto random = static_cast<int>(rng.gen<float>(0.0f, 1.0f) * 16);

				if (i == 8 || i == 12 || i == 16 || i == 20)
				{
					m_id += '-';
				}

				std::stringstream hex;
				hex << std::hex << (i == 12 ? 4 : (i == 16 ? (random & 3 | 8) : random));

				m_id += hex.str();
			}
		}

		Guid::Guid(Guid&& g) noexcept
		{
			this->m_id = std::move(g.m_id);
		}

		Guid& Guid::operator=(Guid&& g) noexcept
		{
			if (this != &g)
			{
				this->m_id = std::move(g.m_id);
			}

			return *this;
		}

		Guid::Guid(const Guid& g) noexcept
		{
			this->m_id = g.m_id;
		}

		Guid& Guid::operator=(const Guid& g) noexcept
		{
			if (this != &g)
			{
				this->m_id = g.m_id;
			}

			return *this;
		}

		Guid::~Guid() noexcept
		{
		}

		const std::string& Guid::to_string() const noexcept
		{
			return m_id;
		}

		bool Guid::operator==(const Guid& rhs) noexcept
		{
			return m_id == rhs.m_id;
		}

		bool Guid::operator!=(const Guid& rhs) noexcept
		{
			return !operator==(rhs);
		}
	} // namespace utils
} // namespace galaxy
