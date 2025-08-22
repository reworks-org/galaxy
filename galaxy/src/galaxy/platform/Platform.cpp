///
/// Platform.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <cstdlib>
#include <ctime>

#include "Platform.hpp"

namespace galaxy
{
	namespace platform
	{
		void seed_random() noexcept
		{
			std::srand(static_cast<unsigned int>(std::time(nullptr)));
		}
	} // namespace platform
} // namespace galaxy
