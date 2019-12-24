///
/// LogColours.hpp
///
/// pulsar
/// See LICENSE.txt.
///

#ifndef PULSAR_LOGCOLOURS_HPP_
#define PULSAR_LOGCOLOURS_HPP_

///
/// Core namespace.
///
namespace pl
{
	///
	/// Enum representing different log text colours.
	///
	enum class LogColours : int
	{
		WHITE = 15,
		GREEN = 10,
		YELLOW = 14,
		RED = 12,
		FATAL = 20
	};
}

#endif