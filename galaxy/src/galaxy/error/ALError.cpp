///
/// ALError.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <AL/al.h>
#include <AL/alc.h>
#include <fmt/format.h>

#include "ALError.hpp"

namespace galaxy
{
	namespace error
	{
		std::string al_parse_error(std::string_view message, const int error)
		{
			return fmt::format("OpenAL: {0} | {1}.", message, alGetString(error));
		}

		std::string al_errcode_as_string(const int err) noexcept
		{
			switch (err)
			{
				case AL_NO_ERROR:
					return "AL_NO_ERROR";
					break;

				case AL_INVALID_NAME:
					return "AL_INVALID_NAME";
					break;

				case AL_INVALID_ENUM:
					return "AL_INVALID_ENUM";
					break;

				case AL_INVALID_VALUE:
					return "AL_INVALID_VALUE";
					break;

				case AL_INVALID_OPERATION:
					return "AL_INVALID_OPERATION";
					break;

				case AL_OUT_OF_MEMORY:
					return "AL_OUT_OF_MEMORY";
					break;

				default:
					return "AL_NO_ERROR";
					break;
			}
		}
	} // namespace error
} // namespace galaxy