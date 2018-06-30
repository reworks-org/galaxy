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
	const std::string Utils::getExtension(const std::string& fileName)
	{
		// Find the last point, which means everything after is the extension, and get a substring of it.
		// Otherwise return an empty string signifying no extension.
		std::size_t lastDot = fileName.find_last_of(".");
		if (lastDot != std::string::npos)
		{
			return fileName.substr(lastDot + 1);
		}
		else
		{
			return "";
		}
	}

	const std::string Utils::removeExtension(const std::string& fileName)
	{
		// Return everything before final dot, since that is everything before the extension.
		// Otherwise just return the file if there is no extension.
		std::size_t lastDot = fileName.find_last_of(".");
		if (lastDot != std::string::npos)
		{
			return fileName.substr(0, lastDot);
		}
		else
		{
			return fileName;
		}
	}

	const std::string Utils::getBaseName(const std::string& pathWithFileName)
	{
		// Return everything after the last path splitters.
		// Otherwise just return the name if there is no path splitting.
		std::size_t base = pathWithFileName.find_last_of("/\\");
		if (base != std::string::npos)
		{
			return pathWithFileName.substr(base + 1);
		}
		else
		{
			return pathWithFileName;
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