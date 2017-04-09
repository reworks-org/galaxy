//
//  Random.hpp
//  REngine3
//
//  Created by reworks on 18/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_RANDOM_HPP_
#define RENGINE3_RANDOM_HPP_

#include <random>

namespace re
{
	namespace Random
	{
		// http://stackoverflow.com/a/32907541
		template <typename T>
		using conditional_distribution = std::conditional_t<std::is_integral<T>::value, std::uniform_int_distribution<T>, std::conditional_t<std::is_floating_point<T>::value, std::uniform_real_distribution<T>, void>>;
		
		/*
		* IMPORTS: min - minimum number inclusive, max - maximum number inclusive. T - type.
		* EXPORTS: none
		* PURPOSE: Generate a random number of type <T>.
		*/
		template<typename T>
		inline T random(T min, T max)
		{
			std::random_device rd;
			std::mt19937 mt(rd());

			conditional_distribution<T> dist(min, max);

			return dist(mt);
		}
}

#endif
