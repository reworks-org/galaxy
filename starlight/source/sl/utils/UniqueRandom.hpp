///
/// UniqueRandom.hpp
/// starlight
///
/// Created by reworks on 07/03/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_UNIQUERANDOM_HPP_
#define STARLIGHT_UNIQUERANDOM_HPP_

#include <any>
#include <random>
#include <algorithm>

namespace sl
{
	struct UniqueRandom
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

			T value = dist(mt);
			auto result = std::find_if(m_usedNumbers.begin(), m_usedNumbers.end(), [&](std::any& any) -> bool
			{
				T other = std::any_cast<T>(any);
				return (value == other);
			});

			if (result != m_usedNumbers.end())
			{
				value = random(min, max);
			}
			
			UniqueRandom::m_usedNumbers.emplace_back(result);

			return value;
		}

		///
		/// Currently used numbers.
		///
		static inline std::vector<std::any> m_usedNumbers;
	};
}

#endif