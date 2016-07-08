//
//  Time.hpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TIME_HPP_
#define RENGINE3_TIME_HPP_

#include <chrono>
#include <cstdint>

namespace re
{
	static inline uint64_t TimeNano()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}

	static inline uint64_t TimeMillis()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
}

#endif