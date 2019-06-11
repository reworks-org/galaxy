///
/// Utils.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_UTILS_HPP_
#define SOLAR_UTILS_HPP_

#include <vector>

namespace sr
{
	struct Utils
	{
		///
		/// Lets push_back support fold expressions easier.
		///
		/// Thanks to: https://www.codingame.com/playgrounds/2205/7-features-of-c17-that-will-simplify-your-code/fold-expressions
		///
		/// \param vector std::vector to push_back to.
		/// \param args Variable amount of objects to push_back.
		///
		template<typename Type, typename... Args>
		static inline void fold_push_back(std::vector<Type>& vector, Args&&... args)
		{
			(vector.push_back(args), ...);
		}

	};
}

#endif