//
//  POSIX.hpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_POSIX_HPP_
#define RENGINE3_POSIX_HPP_

#ifndef _WIN32

#define FOREGROUND_BLUE 0x0001
#define FOREGROUND_GREEN 0x0002
#define FOREGROUND_RED 0x0004
#define FOREGROUND_INTENSITY 0x0008

namespace re
{
	/*
	* IMPORTS: code - color code to use.
	* EXPORTS: none
	* PURPOSE: Change text colour of the console.
	*/
	const char* SetConsoleTextColour(unsigned long code)
	{
        std::string output = "";
        
        switch (code)
        {
            case FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN:
                output = "\x1B[0m";
                break;
                
            case FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY:
                output = "\x1B[33m";
                break;
                
            case FOREGROUND_RED | FOREGROUND_INTENSITY:
                output = "\x1B[31m";
                break;
        }
        
        return output.c_str();
	}
}
#endif

#endif
