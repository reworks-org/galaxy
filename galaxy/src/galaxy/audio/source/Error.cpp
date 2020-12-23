///
/// Error.cpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#include <AL/al.h>
#include <AL/alc.h>
#include <fmt/format.h>

#include "Error.hpp"

///
/// Core namespace.
///
namespace frb
{
	std::string parse_error(std::string_view message)
	{
		return fmt::format("OpenAL: {0} - {1}", message, alGetString(alGetError()));
	}

	std::string errcode_as_string(const int err)
	{
		std::string out {""};
		switch (err)
		{
			case AL_NO_ERROR:
				out = "AL_NO_ERROR";
				break;

			case AL_INVALID_NAME:
				out = "AL_INVALID_NAME";
				break;

			case AL_INVALID_ENUM:
				out = "AL_INVALID_ENUM";
				break;

			case AL_INVALID_VALUE:
				out = "AL_INVALID_VALUE";
				break;

			case AL_INVALID_OPERATION:
				out = "AL_INVALID_OPERATION";
				break;

			case AL_OUT_OF_MEMORY:
				out = "AL_OUT_OF_MEMORY";
				break;

			default:
				out = "AL_NO_ERROR";
				break;
		}

		return out;
	}
} // namespace frb