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

#include "LogColours.hpp"

#undef ERROR

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
	/// \return Returns empty string so function can be used in an output stream mimicing unix style.
	///
	const std::string colourText(const LogColours colour);
}

#endif

#endif