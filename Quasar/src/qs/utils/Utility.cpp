///
/// Utility.cpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <cmath>

#include "Utility.hpp"

namespace qs
{
	Uint8 Utils::fromFloat(float f)
	{
		// Makes sure the float is properly converted to a byte in the correct range.
		return (f >= 1.0 ? 255 : (f <= 0.0 ? 0 : static_cast<Uint8>(std::floor(f * 256.0))));
	}
}