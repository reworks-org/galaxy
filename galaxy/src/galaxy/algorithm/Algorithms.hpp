///
/// Algorithms.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ALGORITHM_ALGORITHMS_HPP_
#define GALAXY_ALGORITHM_ALGORITHMS_HPP_

#include <algorithm>
#include <execution>
#include <vector>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace algorithm
	{
		///
		/// Calc normalized value from range.
		///
		/// \tparam Arithmetic type to normalize.
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
		/// \return True if found.
		///
		template<typename Type>
		[[nodiscard]] inline bool contains(const std::vector<Type>& cont, const Type& val) noexcept
		{
			auto out = false;
			std::for_each(std::execution::par, cont.begin(), cont.end(), [&](const Type& var) {
				if (val == var)
				{
					out = true;
				}
			});

			return out;
		}
	} // namespace algorithm
} // namespace galaxy

#endif