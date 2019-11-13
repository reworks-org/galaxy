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
#include <Windows.h>

#include "../details/LogColours.hpp"

#undef ERROR

namespace pl
{
	///
	/// Holds methods relating to platform specific code.
	///
	struct Platform
	{
		///
		/// Function for defining colour based text on console output.
		///
		/// \param colour Enum defining which colour to colour text in.
		///
		/// \return Returns empty string so function can be used in an output stream mimicing unix style.
		///
		static inline std::string colourText(const LogColours colour)
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, (int)colour);

			return "";
		}
	};
}

#endif

#endif