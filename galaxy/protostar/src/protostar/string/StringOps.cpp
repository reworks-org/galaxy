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
	std::string boolToString(const bool value) noexcept
	{
		return std::move((value == true) ? "true" : "false");
	}

	const bool stringToBool(const std::string& str) noexcept
	{
		return (str == "true") ? true : false;
	}

	const char* nullToEmpty(const char* s) noexcept
	{
		return (s ? s : "");
	}
}