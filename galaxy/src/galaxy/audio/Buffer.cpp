///
/// Buffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <stb/stb_vorbis.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/error/OpenALError.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Buffer.hpp"

namespace galaxy
{
	namespace audio
	{
		Buffer::Buffer()
			: m_buffer {0}
		{
			// Create buffer and check for error. Only create 1 since buffer is being managed per object.
			alGenBuffers(1, &m_buffer);

			const auto error = alGetError();
			if (error != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_FATAL, error::al_handle_error("Unable to gen audio buffer.", error));
			}
		}

		Buffer::~Buffer()
		{
			if (m_buffer > 0)
			{
				alDeleteBuffers(1, &m_buffer);
				m_buffer = 0;

				const auto error = alGetError();
				if (error != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_handle_error("Unable to delete audio buffer(s).", error));
				}
			}
		}

		ALint Buffer::get_frequency() noexcept
		{
			ALint freq;
			alGetBufferi(m_buffer, AL_FREQUENCY, &freq);

			return freq;
		}

		ALint Buffer::get_bits() noexcept
		{
			ALint bits;
			alGetBufferi(m_buffer, AL_BITS, &bits);

			return bits;
		}

		ALint Buffer::get_channels() noexcept
		{
			ALint channels;
			alGetBufferi(m_buffer, AL_CHANNELS, &channels);

			return channels;
		}

		ALint Buffer::get_size() noexcept
		{
			ALint size;
			alGetBufferi(m_buffer, AL_SIZE, &size);

			return size;
		}

		ALuint Buffer::handle() const noexcept
		{
			return m_buffer;
		}

		bool Buffer::internal_load(std::string_view file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto file_info = fs.find(file);

			if (file_info.m_code == fs::FileInfo::Code::FOUND)
			{
				if (file_info.m_path.extension() == ".ogg")
				{
					int channels = 0;
					int samples  = 0;
					short* data  = nullptr;

					const auto length = stb_vorbis_decode_filename(file_info.m_string.c_str(), &channels, &samples, &data);
					if (length < 1)
					{
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

						return false;
					}
					else
					{
						const auto format = (channels > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
						alBufferData(m_buffer, format, data, channels * length * sizeof(short), samples);

						std::free(data);
						return true;
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Audio file must be ogg vorbis and have extension of .ogg!");
					return false;
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find '{0}' because '{1}'.", file, magic_enum::enum_name<fs::FileInfo::Code>(file_info.m_code));
				return false;
			}
		}
	} // namespace audio
} // namespace galaxy