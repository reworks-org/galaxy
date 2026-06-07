///
/// Platform.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <cstdlib>
#include <ctime>

#include <Raylib.hpp>

#include "Platform.hpp"

namespace galaxy
{
	namespace platform
	{
		void seed_random() noexcept
		{
			const auto seed = static_cast<unsigned int>(std::time(nullptr));
			std::srand(seed);
			ray::SetRandomSeed(seed);
		}
	} // namespace platform
} // namespace galaxy
