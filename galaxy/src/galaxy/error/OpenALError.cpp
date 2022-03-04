///
/// OpenALError.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <format>

#include <AL/al.h>
#include <AL/alc.h>

#include "OpenALError.hpp"

namespace galaxy
{
	namespace error
	{
		std::string al_handle_error(std::string_view message, const int err)
		{
			// Generate a proper openal formatted message.
			auto formatted = std::format("OpenAL: {0} | {1}.", message, al_errcode_as_string(err));

			// Reset openal error state.
			alGetError();

			return formatted;
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

		std::string alc_handle_error(ALCdevice* device, std::string_view message, const int err)
		{
			// Generate a proper openal formatted message.
			auto formatted = std::format("OpenAL: {0} | {1}.", message, alc_errcode_as_string(err));

			// Reset openal error state.
			alcGetError(device);

			return formatted;
		}

		std::string alc_errcode_as_string(const int err) noexcept
		{
			switch (err)
			{
				case ALC_NO_ERROR:
					return "ALC_NO_ERROR";
					break;

				case ALC_INVALID_CONTEXT:
					return "ALC_INVALID_CONTEXT";
					break;

				case ALC_INVALID_ENUM:
					return "ALC_INVALID_ENUM";
					break;

				case ALC_INVALID_VALUE:
					return "ALC_INVALID_VALUE";
					break;

				case ALC_OUT_OF_MEMORY:
					return "ALC_OUT_OF_MEMORY";
					break;

				default:
					return "ALC_NO_ERROR";
					break;
			}
		}
	} // namespace error
} // namespace galaxy