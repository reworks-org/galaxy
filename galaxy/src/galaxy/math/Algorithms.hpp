///
/// Algorithms.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_ALGORITHMS_HPP_
#define GALAXY_MATH_ALGORITHMS_HPP_

#include <algorithm>
#include <execution>
#include <vector>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace math
	{
		///
		/// Calc normalized value from range.
		///
		/// \param val Value to divide by max.
		/// \param max Value representing 1.0f.
		///
		/// \return Normalized float.
		///
		template<meta::is_arithmetic Type>
		[[nodiscard]] inline float normalize(const Type val, const Type max) noexcept
		{
			return (static_cast<float>(val) / static_cast<float>(max));
		}

		///
		/// See if a vector contains a value.
		///
		/// \tparam Type Vector type. Must have an overloaded '==' operator.
		///
		/// \param cont Container to check.
		/// \param val Value to look for.
		///
		/// \return Either iterator pointing to the end or to the value.
		///
		template<typename Type>
		[[nodiscard]] inline bool contains(const std::vector<Type>& cont, const Type& val) noexcept
		{
			std::for_each(std::execution::par, cont.begin(), cont.end(), [&](const Type& var) {
				if (val == var)
				{
					return true;
				}
			});

			return false;
		}
	} // namespace math
} // namespace galaxy

#endif