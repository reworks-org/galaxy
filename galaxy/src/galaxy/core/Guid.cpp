///
/// Guid.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <format>
#include <random>

#include "Guid.hpp"

namespace galaxy
{
	namespace core
	{
		Guid::Guid() noexcept
		{
			thread_local std::random_device s_device;
			thread_local std::mt19937_64 s_engine(s_device());
			thread_local std::uniform_int_distribution<std::uint64_t> s_64dist;
			thread_local std::uniform_int_distribution<std::uint32_t> s_32dist;
			thread_local std::uniform_int_distribution<std::uint16_t> s_16dist;
			thread_local std::uniform_int_distribution<std::uint16_t> s_8dist(0, 255);

			m_first  = s_32dist(s_engine);
			m_second = s_16dist(s_engine);
			m_third  = static_cast<std::uint8_t>(s_8dist(s_engine));
			m_fourth = static_cast<std::uint8_t>(s_8dist(s_engine));
			m_fifth  = s_64dist(s_engine);
			m_string = std::format("{0}-{1}-{2}-{3}-{4}", m_first, m_second, m_third, m_fourth, m_fifth);
		}

		Guid::Guid(Guid&& g) noexcept
		{
			this->m_first  = g.m_first;
			this->m_second = g.m_second;
			this->m_third  = g.m_third;
			this->m_fourth = g.m_fourth;
			this->m_fifth  = g.m_fifth;

			this->m_string = g.m_string;
		}

		Guid& Guid::operator=(Guid&& g) noexcept
		{
			if (this != &g)
			{
				this->m_first  = g.m_first;
				this->m_second = g.m_second;
				this->m_third  = g.m_third;
				this->m_fourth = g.m_fourth;
				this->m_fifth  = g.m_fifth;

				this->m_string = g.m_string;
			}

			return *this;
		}

		const std::string& Guid::to_string() const noexcept
		{
			return m_string;
		}

		bool Guid::is_empty() const noexcept
		{
			if ((m_first == 0) && (m_second == 0) && (m_third == 0) && (m_fourth == 0) && (m_fifth == 0))
			{
				return true;
			}

			return false;
		}

		bool Guid::operator==(const Guid& rhs) noexcept
		{
			return (m_first == rhs.m_first) && (m_second == rhs.m_second) && (m_third == rhs.m_third) && (m_fourth == rhs.m_fourth) && (m_fifth == rhs.m_fifth);
		}

		bool Guid::operator!=(const Guid& rhs) noexcept
		{
			return !operator==(rhs);
		}
	} // namespace core
} // namespace galaxy