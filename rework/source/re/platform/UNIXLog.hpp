//
//  UNIXLog.hpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_UNIXLOG_HPP_
#define RENGINE3_UNIXLOG_HPP_

#ifndef _WIN32

namespace re
{
	///
	/// Change the colour of the text being printed to the console.
	///
	/// \param code ACSII RE_COLOUR code to use.
	///
	inline const char* setConsoleTextColour(const char* code)
	{
		return code;
	}
}
#endif

#endif
