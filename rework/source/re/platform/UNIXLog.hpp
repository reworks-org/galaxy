//
//  UNIXLog.hpp
//  rework
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_UNIXLOG_HPP_
#define REWORK_UNIXLOG_HPP_

#ifndef _WIN32

namespace re
{
	///
	/// Change the colour of the text being printed to the console.
	///
	/// \param code ACSII RE_<COLOUR> code to use.
	///
	inline constexpr const char* setConsoleTextColour(const char* code)
	{
		return code;
	}
}
#endif

#endif