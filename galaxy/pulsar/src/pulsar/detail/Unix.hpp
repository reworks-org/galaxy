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

#include "LogColours.hpp"

///
/// Core namespace.
///
namespace pulsar
{
	///
	/// Function for defining colour based text on console output.
	///
	/// \param colour Enum defining which colour to colour text in.
	///
	/// \return Returns string ascii code to colour text.
	///
	std::string colourText(const LogColours colour) noexcept;
}

#endif

#endif