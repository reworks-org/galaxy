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
	const std::string Utils::boolToString(const bool value)
	{
		std::string out = (value == true) ? "true" : "false";
		return out;

	}

	const bool Utils::stringToBool(const std::string& str)
	{
		bool out = (str == "true") ? true : false;
		return out;
	}

	const char* Utils::nullToEmpty(const char* s)
	{
		return (s ? s : "");
	}
}