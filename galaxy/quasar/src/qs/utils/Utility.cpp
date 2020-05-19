///
/// Utility.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <cmath>

#include "Utility.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Utility functions used by Quasar.
	///
	namespace Utils
	{
		std::uint8_t floatToUint8(float f) noexcept
		{
			// Makes sure the float is properly converted to a byte in the correct range.
			return (f >= 1.0 ? 255 : (f <= 0.0 ? 0 : static_cast<std::uint8_t>(std::floor(f * 256.0))));
		}

		float uint8ToFloat(std::uint8_t u8) noexcept
		{
			// Ensure proper conversion and no rounding errors.
			return (u8 >= 255 ? 1.0f : (u8 == 0 ? 0.0f : static_cast<std::uint8_t>(std::floor(u8 / 256.0))));
		}
	}
}