///
/// Random.hpp
/// galaxy
///
/// Created by reworks on 18/07/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RANDOM_HPP_
#define GALAXY_RANDOM_HPP_

#include <random>

namespace galaxy
{
	///
	/// Contains static random number generation functions.
	///
	struct Random
	{	
		///
		/// http://stackoverflow.com/a/32907541
		///
		template<typename T>
		using conditional_distribution = std::conditional_t<std::is_integral<T>::value, std::uniform_int_distribution<T>, std::conditional_t<std::is_floating_point<T>::value, std::uniform_real_distribution<T>, void>>;

		///
		/// Generate a random number of type T.
		///
		/// \param min Minimum number inclusive.
		/// \param max Maximum number inclusive.
		///
		/// \return Returns type T.
		///
		template<typename T>
		static inline T random(const T min, const T max)
		{
			std::random_device rd;
			std::mt19937 mt(rd());

			conditional_distribution<T> dist(min, max);

			return dist(mt);
		}
	};
}

#endif