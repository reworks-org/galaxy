//
//  Utils.hpp
//  rework
//
//  Created by reworks on 26/06/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_UTILS_HPP_
#define REWORK_UTILS_HPP_

#include <string>
#include <sstream>

#include "loguru/loguru.hpp"

namespace re
{
	namespace Utils
	{
		///
		/// \brief Get an extension.
		///
		/// Thanks to: https://stackoverflow.com/a/4505931
		///
		/// \param filename Name of file with an extension to retrieve. ONLY WORKS FOR FILES. DO NOT USE FOR PATHS! ESPECIALLY PATHS WITH DOTS IN THEM!
		///
		/// \return string containing extension.
		///
		inline std::string getExtension(const std::string& filename)
		{
			if (filename.find_last_of(".") != std::string::npos)
			{
				return filename.substr(filename.find_last_of(".") + 1);
			}
			else
			{
				return "";
			}
		}

		///
		/// Converts a boolean to a std::string.
		///
		/// \param value Boolean value to convert.
		///
		/// \return Returns either "true" or "false".
		///
		inline std::string boolToString(bool value)
		{
			std::string out = (value == true) ? "true" : "false";
			return out;
			
		}

		///
		/// Converts a std::string to a boolean.
		///
		/// \param value std::string value to convert.
		///
		/// \return Returns either true or false.
		///
		inline constexpr bool stringToBool(const std::string& str)
		{
			bool out = (str == "true") ? true : false;
			return out;
		}

		///
		/// \brief Convert a std::string to any type.
		///
		/// Thanks to: https://gist.github.com/timofurrer/2725779
		/// Some adjustments have been made.
		///
		/// \param data std::string containing type to convert.
		///
		/// \return Returns data as T type.
		///
		template <typename T>
		inline T convertString(const std::string &data)
		{
			if (!data.empty())
			{
				T ret;

				std::istringstream iss(data);
				if (data.find("0x") != std::string::npos)
				{
					iss >> std::hex >> ret;
				}
				else
				{
					iss >> std::dec >> ret;
				}

				if (iss.fail())
				{
					LOG_S(FATAL) << "Convert error: cannot convert string '" << data << "' to value" << std::endl;
					return T();
				}
				return ret;
			}
			return T();
		}

		///
		/// Template specialization for convertString for std::string.
		///
		/// \see convertString
		///
		template<>
		inline std::string convertString<std::string>(const std::string& data)
		{
			return data;
		}

		///
		/// Template specialization for convertString for bool.
		///
		/// \see convertString
		///
		template<>
		inline bool convertString<bool>(const std::string& data)
		{
			return Utils::stringToBool(data);
		}

		///
		/// \brief Ensures any null c-strings are converted to empty ones.
		///
		/// Credits: https://stackoverflow.com/a/1724514
		///
		/// \param s String to check.
		///
		/// \return Returns empty string if input is null.
		///
		///
		inline constexpr const char* nullToEmpty(char const* s)
		{
			return (s ? s : "");
		}
	}
}

#endif