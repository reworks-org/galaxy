///
/// Random.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ALGORITHM_RANDOM_HPP_
#define GALAXY_ALGORITHM_RANDOM_HPP_

#include <random>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace algorithm
	{
		///
		/// Credits: http://stackoverflow.com/a/32907541.
		///
		template<typename Type>
		using conditional_distribution = std::conditional_t<
		    std::is_integral<Type>::value,
		    std::uniform_int_distribution<Type>,
		    std::conditional_t<std::is_floating_point<Type>::value, std::uniform_real_distribution<Type>, void>>;

		///
		/// Generate a random number of type T.
		///
		/// \param min Minimum number inclusive.
		/// \param max Maximum number inclusive.
		///
		/// \return Returns number of the same type as inputs.
		///
		template<meta::is_arithmetic Type>
		[[nodiscard]] inline Type random(const Type min, const Type max)
		{
			std::random_device rd;
			std::mt19937_64 mt {rd()};

			conditional_distribution<Type> dist {min, max};

			return dist(mt);
		}
	} // namespace algorithm
} // namespace galaxy

#endif