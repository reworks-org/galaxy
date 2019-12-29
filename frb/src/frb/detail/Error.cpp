///
/// Error.cpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>

#include "Error.hpp"

namespace frb
{
	const std::string parseError(const std::string& message) noexcept
	{
		return "OpenAL Error: " + message + " | " + alGetString(alGetError());
	}
}