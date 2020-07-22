///
/// Time.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>

#include "Time.hpp"

///
/// Core namespace.
///
namespace pr
{
	const double time_since_epoch() noexcept
	{
		// Return time since epoch using chrono.
		return std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}
} // namespace pr