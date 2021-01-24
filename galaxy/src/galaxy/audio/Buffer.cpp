///
/// Buffer.cpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <stb/stb_vorbis.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/ALError.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "Buffer.hpp"

namespace galaxy
{
	namespace audio
	{
		Buffer::Buffer()
		    : m_buffer {0}
		{
			// Create buffer and check for error. Only create 1 bffer since buffer is being managed per object.
			alGenBuffers(1, &m_buffer);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_FATAL, error::al_parse_error("Unable to gen audio buffer."));
			}
		}

		Buffer::~Buffer()
		{
			alDeleteBuffers(1, &m_buffer);
		}

		const ALint Buffer::get_frequency()
		{
			ALint freq = 0;
			alGetBufferi(m_buffer, AL_FREQUENCY, &freq);

			return freq;
		}

		const ALint Buffer::get_bits()
		{
			ALint bits = 0;
			alGetBufferi(m_buffer, AL_BITS, &bits);

			return bits;
		}

		const ALint Buffer::get_channels()
		{
			ALint channels = 0;
			alGetBufferi(m_buffer, AL_CHANNELS, &channels);

			return channels;
		}

		const ALint Buffer::get_size()
		{
			ALint size = 0;
			alGetBufferi(m_buffer, AL_SIZE, &size);

			return size;
		}

		const ALuint Buffer::handle() const noexcept
		{
			return m_buffer;
		}

		const bool Buffer::internal_load(std::string_view file)
		{
			bool result = true;
			auto path   = SL_HANDLE.vfs()->absolute(file);

			if (std::filesystem::path(path).extension() != ".ogg")
			{
				GALAXY_LOG(GALAXY_ERROR, "Sound must be ogg vorbis and have extension of .ogg!");
				return result;
			}
			else
			{
				int channels = 0;
				int samples  = 0;
				short* data  = nullptr;

				const auto length = stb_vorbis_decode_filename(path.c_str(), &channels, &samples, &data);
				if (length < 1)
				{
					result = false;

					// Make sure data is freed.
					if (data != nullptr)
					{
						std::free(data);
					}

					if (length == -1)
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to open file with stb_vorbis.");
					}
					else if (length == -2)
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to parse with stb_vorbis.");
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed due to unknown error. Error code returned: {0}.", length);
					}
				}
				else
				{
					const auto format = (channels > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
					alBufferData(m_buffer, format, data, channels * length * sizeof(short), samples);

					std::free(data);
				}
			}

			return result;
		}
	} // namespace audio
} // namespace galaxy