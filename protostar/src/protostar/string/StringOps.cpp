///
/// StringOps.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "StringOps.hpp"

///
/// Core namespace.
///
namespace protostar
{
	const std::string boolToString(const bool value)
	{
		return (value == true) ? "true" : "false";
	}

	const bool stringToBool(const std::string& str)
	{
		return (str == "true") ? true : false;
	}

	const char* nullToEmpty(const char* s)
	{
		return (s ? s : "");
	}
}