///
/// System.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "System.hpp"

namespace galaxy
{
	namespace systems
	{
		System::System(const std::string& id) noexcept
			: m_id {id}
		{
		}

		System::~System() noexcept
		{
		}

		const std::string& System::id() const noexcept
		{
			return m_id;
		}
	} // namespace systems
} // namespace galaxy
