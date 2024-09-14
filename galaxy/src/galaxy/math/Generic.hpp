///
/// Generic.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_GENERIC_HPP_
#define GALAXY_MATH_GENERIC_HPP_

#include <algorithm>
#include <vector>

#include "galaxy/utils/Concepts.hpp"

namespace galaxy
{
	namespace math
	{
		///
		/// Calc normalized value from range.
		///
		/// \tparam Type Arithmetic type to normalize.
		///
		/// \param val Value to divide by max.
		/// \param max Value representing 1.0f.
		///
		/// \return Normalized float.
		///
		template<utils::is_arithmetic Type>
		[[nodiscard]]
		inline float normalize(const Type val, const Type max)
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
		[[nodiscard]]
		inline bool contains(const std::vector<Type>& cont, const Type& val)
		{
			auto out = false;
			std::for_each(cont.begin(), cont.end(), [&](const Type& var) {
				if (val == var)
				{
					out = true;
				}
			});

			return out;
		}
	} // namespace math
} // namespace galaxy

#endif
