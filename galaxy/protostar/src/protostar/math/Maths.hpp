//
/// Maths.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_MATHS_HPP_
#define PROTOSTAR_MATHS_HPP_

#include "protostar/system/Concepts.hpp"

///
/// Core namespace.
///
namespace pr
{
	///
	/// Calc normalized value from range.
	///
	/// \param val Value to divide by max.
	/// \param max Value representing 1.0f.
	///
	/// \return Normalized float.
	///
	inline float normalize(pr::is_arithmetic auto val, pr::is_arithmetic auto max) noexcept
	{
		return (static_cast<float>(val) / static_cast<float>(max));
	}
} // namespace pr

#endif