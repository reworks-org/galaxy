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

#include "../details/LogColours.hpp"

namespace pl
{
	struct Platform
	{
		static inline std::string colourText(const LogColours colour)
		{
			std::string out = "";

			switch (colour)
			{
			case LogColours::WHITE:
				out = "\x1B[37m";
				break;

			case LogColours::GREEN:
				out = "\x1B[32m";
				break;

			case LogColours::YELLOW:
				out = "\x1B[33m";
				break;

			case LogColours::RED:
				out = "\x1B[31m";
				break;

			case LogColours::FATAL:
				out = "\x1B[31m\x1B[47m";
				break;

			default:
				out = "\x1B[37m";
				break;
			}

			return out;
		}
	};
}

#endif

#endif