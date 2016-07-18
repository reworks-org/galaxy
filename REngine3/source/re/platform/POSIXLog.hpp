//
//  POSIX.hpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_POSIX_HPP_
#define RENGINE3_POSIX_HPP_

//"\033[" << mod.code << "m";

#ifndef _WIN32
namespace re
{
	/*
	* IMPORTS: code - color code to use.
	* EXPORTS: none
	* PURPOSE: Change text colour of the console.
	*/
	const char* SetConsoleTextColour(unsigned long code)
	{

	}
}
#endif

#endif