///
/// MaterialBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "MaterialBook.hpp"

namespace galaxy
{
	namespace res
	{
		MaterialBook::~MaterialBook() noexcept
		{
			clear();
		}

		void MaterialBook::clear() noexcept
		{
			m_resources.clear();
		}
	} // namespace res
} // namespace galaxy