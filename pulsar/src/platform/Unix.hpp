///
/// Unix.hpp
///
/// pulsar
/// See LICENSE.txt.
///

#ifndef PULSAR_UNIX_HPP_
#define PULSAR_UNIX_HPP_

#if defined(__APPLE__) || defined(__linux__) || defined(__unix__)

#include <string>

namespace pl
{
	struct Unix
	{
		static inline std::string colourText(const std::string& text, const std::string& colour)
		{

		}
	};
}

#endif

#endif