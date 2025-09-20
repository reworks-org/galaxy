///
/// System.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "System.hpp"

namespace galaxy
{
	System::System(const std::string& id) noexcept
		: m_id {id}
	{
	}

	System::System(System&& s) noexcept
	{
		this->m_id = std::move(s.m_id);
	}

	System& System::operator=(System&& s) noexcept
	{
		if (this != &s)
		{
			this->m_id = std::move(s.m_id);
		}

		return *this;
	}

	System::~System() noexcept
	{
	}

	const std::string& System::id() const noexcept
	{
		return m_id;
	}
} // namespace galaxy
