///
/// Storage.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Storage.hpp"

namespace galaxy
{
	namespace events
	{
		Storage::Storage(Storage&& s) noexcept
		{
			this->m_event_functions = std::move(s.m_event_functions);
		}

		Storage& Storage::operator=(Storage&& s) noexcept
		{
			if (this != &s)
			{
				this->m_event_functions = std::move(s.m_event_functions);
			}

			return *this;
		}

	} // namespace events
} // namespace galaxy