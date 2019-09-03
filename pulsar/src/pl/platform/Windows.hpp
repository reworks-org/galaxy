///
/// Windows.hpp
///
/// pulsar
/// See LICENSE.txt.
///

#ifndef PULSAR_WINDOWS_HPP_
#define PULSAR_WINDOWS_HPP_

#if defined(_WIN64) || defined(_WIN32)

#include <string>

namespace pl
{
	struct Windows
	{
		static inline std::string colourText(const std::string& text, const std::string& colour)
		{

		}
	};
}

#endif

#endif