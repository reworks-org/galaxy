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
	///
	/// Container structure for utility functions.
	///
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

		///
		/// Returns all duplicate integers that occur a number of times.
		///
		/// Thanks to: https://thispointer.com/c-how-to-find-duplicates-in-a-vector/
		///
		template<typename Type>
		static inline std::vector<Type> findDuplicates(const std::vector<Type>& input, const unsigned int occurances)
		{
			// Counted elements.
			std::unordered_map<Type, unsigned int> counted;
			std::vector<Type> output;

			for (auto& elem : input)
			{
				auto result = counted.insert(std::pair<Type, unsigned int>(elem, 1));
				if (result.second == false)
				{
					result.first->second++;
				}
			}

			for (auto& it : counted)
			{
				if ((it.second < occurances) == false)
				{
					output.push_back(it.first);
				}
			}

			return output;
		}

	};
}

#endif