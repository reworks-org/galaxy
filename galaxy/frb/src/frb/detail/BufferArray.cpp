///
/// BufferArray.cpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#include "BufferArray.hpp"

///
/// Core namespace.
///
namespace frb
{
	BufferArray::~BufferArray() noexcept
	{
		for (auto count = 0; count < m_bufferArray.size(); count++)
		{
			if (m_bufferArray[count] != static_cast<ALuint>(-1))
			{
				alDeleteBuffers(1, &m_bufferArray[count]);
			}
		}

		m_bufferArray.clear();
	}

	bool BufferArray::load(const std::vector<std::string>& files) noexcept
	{
		bool result = true;

		// Create buffer and check for error. Only create 1 bffer since buffer is being managed per object.
		alGenBuffers(static_cast<ALsizei>(files.size()), m_bufferArray.data());
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to gen audio buffer."));
		}

		for (ALuint count = 0; count < m_bufferArray.size(); count++)
		{
			result = iloadFile(files[count], count);
		}

		return result;
	}

	bool BufferArray::load(const std::vector<std::pair<unsigned char*, const int>>& data) noexcept
	{
		bool result = true;

		// Create buffer and check for error. Only create 1 bffer since buffer is being managed per object.
		alGenBuffers(static_cast<ALsizei>(data.size()), m_bufferArray.data());
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to gen audio buffer."));
		}

		for (ALuint count = 0; count < m_bufferArray.size(); count++)
		{
			result = iloadMem(data[count].first, data[count].second, count);
		}

		return result;
	}

	std::vector<ALuint>& BufferArray::raw() noexcept
	{
		return m_bufferArray;
	}

	void BufferArray::destroy() noexcept
	{
		alDeleteBuffers(static_cast<ALsizei>(m_bufferArray.size()), m_bufferArray.data());
		for (auto count = 0; count < m_bufferArray.size(); count++)
		{
			m_bufferArray[count] = static_cast<ALuint>(-1);
		}
	}

	bool BufferArray::iloadFile(const std::string& file, const ALuint buffer) noexcept
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
				alBufferData(buffer, format, data, channels * length * sizeof(short), samples);

				std::free(data);
			}
		}

		return result;
	}

	bool BufferArray::iloadMem(const unsigned char* mem, const int size, const ALuint buffer) noexcept
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
			alBufferData(buffer, format, data, channels * length * sizeof(short), samples);

			std::free(data);
		}

		return result;
	}
}