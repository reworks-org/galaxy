///
/// Utils.cpp
/// starlight
///
/// Created by reworks on 26/06/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "Utils.hpp"

namespace sl
{
	const std::string Utils::getExtension(const std::string& filename)
	{
		// Find the last point, which means everything after is the extension, and get a substring of it.
		if (filename.find_last_of(".") != std::string::npos)
		{
			return filename.substr(filename.find_last_of(".") + 1);
		}
		else
		{
			return "";
		}
	}

	const std::string Utils::removeExtension(const std::string& filename)
	{
		// Return everything before final dot, since that is everything before the extension.
		size_t lastdot = filename.find_last_of(".");
		if (lastdot == std::string::npos)
		{
			return filename;
		}
		else
		{
			return filename.substr(0, lastdot);
		}
	}

	const std::string Utils::boolToString(bool value)
	{
		std::string out = (value == true) ? "true" : "false";
		return out;

	}

	const bool Utils::stringToBool(const std::string& str)
	{
		bool out = (str == "true") ? true : false;
		return out;
	}

	const char* Utils::nullToEmpty(char const* s)
	{
		return (s ? s : "");
	}
}