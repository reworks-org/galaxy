//
//  Utils.hpp
//  REngine3
//
//  Created by reworks on 26/06/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_UTILS_HPP_
#define RENGINE3_UTILS_HPP_

#include <string>

namespace re
{
	namespace Utils
	{
		/*
		* IMPORTS: bool value to write out.
		* EXPORTS: std::string of bool
		* PURPOSE: "convert" a boolean value to a std::string
		*/
		inline std::string boolToStr(bool value)
		{
			if (value == true)
			{
				return "true";
			}
			else
			{
				return "false";
			}
		}

		/*
		* IMPORTS: std::string value to write out.
		* EXPORTS: bool of std::string
		* PURPOSE: "convert" a std::string value to a boolean
		*/
		inline bool strToBool(const std::string& str)
		{
			if (str == "true")
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}

#endif
