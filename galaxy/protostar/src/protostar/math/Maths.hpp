///
/// Maths.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_MATHS_HPP_
#define PROTOSTAR_MATHS_HPP_

#include <list>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <unordered_map>

#include "protostar/system/Concepts.hpp"

///
/// Core namespace.
///
namespace pr
{
	///
	/// \brief Calculate the percentage in a custom range.
	///
	/// Credits: https://stackoverflow.com/a/25835683
	///
	/// \param input The value to check the percentage of.
	/// \param min The value representing 0%.
	/// \param max The value representing 100%.
	/// 
	/// \return Returns the percentage of the value in that range betwen 0 and 1. E.g. 39% is 0.39.
	///
	template<IsArithmetic Type>
	inline constexpr Type percentInRange(Type input, Type min, Type max) noexcept
	{
		return ((input - min) / (max - min));
	}

	///
	/// \brief Iterates over a std::vector only keeps duplicates.
	///
	/// Credits: https://stackoverflow.com/a/58499766
	///		     https://stackoverflow.com/questions/58498959/keep-the-duplicated-values-only-vectors-c#comment103362833_58499766
	///
	template <typename T>
	inline void only_distinct_duplicates(std::vector<T>& v)
	{
		static_assert(std::is_arithmetic<T>::value);

		std::sort(v.begin(), v.end());

		auto output = v.begin();
		auto first = v.begin();
		const auto last = v.end();
		while (first != last) {
			const auto next{ std::find_if(first + 1, last, [&](const T& t) { return t != *first; }) };
			if (next - first > 1) {
				*output++ = *first;
			}
			first = next;
		}

		v.erase(output, last);
	}

	///
	/// \brief Count intersections across a list of containers.
	///
	/// Based on: https://stackoverflow.com/a/52170516
	///
	/// \param containers Containers to check for intersections.
	///
	/// \return Move constructible std::vector containing intersecting values.
	///
	template<typename T>
	inline std::vector<T> intersections(std::list<std::vector<T>*>& containers) noexcept
	{
		std::unordered_map<T, int> table;
		auto count = containers.size();

		for (const auto* vector : containers)
		{
			for (const auto& element : *vector)
			{
				table[element] += 1;
			}
		}

		std::vector<T> out;
		for (const auto& pair : table)
		{
			if (pair.second == count)
			{
				out.push_back(pair.first);
			}
		}

		return out;
	}
}

#endif