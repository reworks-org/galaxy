///
/// Math.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_MATH_HPP_
#define GALAXY_MATH_MATH_HPP_

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace math
	{
		///
		/// Calc normalized value from range.
		///
		/// \tparam Arithmetic Type to normalize.
		///
		/// \param val Value to divide by max.
		/// \param max Value representing 1.0f. I.e. 255.
		///
		/// \return Normalized float.
		///
		template<meta::is_arithmetic Arithmetic>
		[[nodiscard]]
		inline float constexpr normalize(const Arithmetic val, const Arithmetic max) noexcept
		{
			if (val != 0 && val != max)
			{
				return static_cast<float>(val) / static_cast<float>(max);
			}
			else if (val == max)
			{
				return 1.0f;
			}
			else
			{
				return 0.0f;
			}
		}
	} // namespace math
} // namespace galaxy

#endif
