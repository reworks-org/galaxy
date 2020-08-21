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
	template<pr::is_arithmetic Type>
	inline float normalize(const Type val, const Type max) noexcept
	{
		return (static_cast<float>(val) / static_cast<float>(max));
	}
} // namespace pr

#endif