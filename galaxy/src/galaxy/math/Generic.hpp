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
		inline float normalize(const Type val, const Type max) noexcept
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
		inline bool contains(const std::vector<Type>& cont, const Type& val) noexcept
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

		///
		/// Normalize input value within input range.
		///
		/// \param value The value to normalize.
		/// \param start The starting range value.
		/// \param end The ending range value.
		///
		/// \return The normalized value (0.0 to 1.0).
		///
		[[nodiscard]]
		float normalize(const float value, const float start, const float end) noexcept;

		///
		/// Remap input value within input range to output range.
		///
		/// \param value The value to remap.
		/// \param input_start The start of the input range.
		/// \param input_end The end of the input range.
		/// \param output_start The start of the output range.
		/// \param output_end The end of the output range.
		///
		/// \return The remapped value.
		///
		[[nodiscard]]
		float remap(const float value, const float input_start, const float input_end, const float output_start, const float output_end) noexcept;

		///
		/// Wrap input value from min to max.
		///
		/// \param value The value to wrap.
		/// \param min The minimum value of the range.
		/// \param max The maximum value of the range.
		///
		/// \return The wrapped value.
		///
		[[nodiscard]]
		float wrap(const float value, const float min, const float max) noexcept;
	} // namespace math
} // namespace galaxy

#endif
