///
/// Buffer.cpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <stb_vorbis.h>
#include <pulsar/Log.hpp>

#include "frb/Error.hpp"

#include "Buffer.hpp"

///
/// Core namespace.
///
namespace frb
{
	Buffer::Buffer() noexcept
		:m_buffer(0)
	{
		// Create buffer and check for error. Only create 1 bffer since buffer is being managed per object.
		alGenBuffers(1, &m_buffer);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_FATAL, frb::parseError("Unable to gen audio buffer."));
		}
	}

	Buffer::~Buffer() noexcept
	{
		destroy();
	}

	bool Buffer::load(const std::string& file) noexcept
	{
		bool result = true;

		// Process filepath properly.
		auto path = std::filesystem::path(file);
		if (path.extension() != ".ogg")
		{
			PL_LOG(PL_ERROR, "File must be ogg vorbis and have extension of .ogg!");
			result = false;
		}
		else
		{
			int channels = 0;
			int samples = 0;
			short* data = nullptr;

			auto length = stb_vorbis_decode_filename(path.string().c_str(), &channels, &samples, &data);
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
					PL_LOG(PL_ERROR, "Failed to open file with stb_vorbis.");
				}
				else if (length == -2)
				{
					PL_LOG(PL_ERROR, "Failed to parse with stb_vorbis.");
				}
				else
				{
					PL_LOG(PL_ERROR, "Failed due to unknown error. Error code returned: " + std::to_string(length));
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

	bool Buffer::load(const unsigned char* mem, const int size) noexcept
	{
		int channels = 0;
		int samples = 0;
		short* data = nullptr;
		bool result = true;

		const auto length = stb_vorbis_decode_memory(mem, size, &channels, &samples, &data);
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
				PL_LOG(PL_ERROR, "Failed to open file with stb_vorbis.");
			}
			else if (length == -2)
			{
				PL_LOG(PL_ERROR, "Failed to parse with stb_vorbis.");
			}
			else
			{
				PL_LOG(PL_ERROR, "Failed due to unknown error. Error code returned: " + std::to_string(length));
			}
		}
		else
		{
			const auto format = (channels > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
			alBufferData(m_buffer, format, data, channels * length * sizeof(short), samples);

			std::free(data);
		}

		return result;
	}

	const ALint Buffer::getFrequency() noexcept
	{
		ALint freq = 0;
		alGetBufferi(m_buffer, AL_FREQUENCY, &freq);

		return std::move(freq);
	}

	const ALint Buffer::getBits() noexcept
	{
		ALint bits = 0;
		alGetBufferi(m_buffer, AL_BITS, &bits);

		return std::move(bits);
	}

	const ALint Buffer::getChannels() noexcept
	{
		ALint channels = 0;
		alGetBufferi(m_buffer, AL_CHANNELS, &channels);

		return std::move(channels);
	}

	const ALint Buffer::getSize() noexcept
	{
		ALint size = 0;
		alGetBufferi(m_buffer, AL_SIZE, &size);

		return std::move(size);
	}

	const ALuint Buffer::handle() const noexcept
	{
		return m_buffer;
	}

	void Buffer::destroy() noexcept
	{
		alDeleteBuffers(1, &m_buffer);
		m_buffer = 0;
	}
}