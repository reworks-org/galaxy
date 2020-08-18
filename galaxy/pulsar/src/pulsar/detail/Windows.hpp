///
/// Windows.hpp
///
/// pulsar
/// See LICENSE.txt.
///

#ifndef PULSAR_WINDOWS_HPP_
#define PULSAR_WINDOWS_HPP_

#if defined(_WIN64)

#include <string>

#include "LogColours.hpp"

///
/// Core namespace.
///
namespace pl
{
	///
	/// Function for defining colour based text on console output.
	///
	/// \param colour Enum defining which colour to colour text in.
	///
	/// \return Returns empty string so function can be used in an output stream mimicing unix style.
	///
	[[maybe_unused]] std::string colour_text(const LogColours colour) noexcept;
} // namespace pl

#endif

#endif